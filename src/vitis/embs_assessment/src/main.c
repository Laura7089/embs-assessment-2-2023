#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include "field.h"
#include "solve.h"

const unsigned int MSG_HEADER_SIZE = 6;
ip_addr_t PUZZLE_SERVER_IP;
unsigned short PUZZLE_SERVER_PORT = 51100;
unsigned char MAC_ADDR[] = {0x00, 0x11, 0x22, 0x33, 0x00, 0x26};

uint32_t seed_from_bytes(char* bytes) {
	return 0; // TODO
}

void bytes_from_seed(char* buf, uint32_t seed) {
	for (int i = 3; i >= 0; i--) {
		uint32_t mask = 0b11111111 << i;
		buf[3-i] = (seed & mask) >> i;
	}
}

void make_and_send_puzzle_req(struct udp_pcb *send_pcb) {
	// Assemble the request
	char size;
	printf("\nEnter puzzle size");
	scanf("%hhu", &size);
	while (size < 2 || size > 8) {
		printf("\nIllegal size %d, please enter again", size);
		scanf("%hhu", &size);
	}
	printf("\nEnter random seed");
	uint32_t seed;
	scanf("%lu", &seed);

	char request_msg[6];
	request_msg[0] = 1;
	request_msg[1] = size;
	bytes_from_seed(&request_msg[2], seed);

	// Create a packet buffer and set the payload as the message
	printf("\nSending [%d, %d, %d, %d, %d, %d]\n", request_msg[0], request_msg[1], request_msg[2], request_msg[3], request_msg[4], request_msg[5]);
    struct pbuf * request = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_REF);
    request->payload = request_msg;
	// Send the message
	udp_sendto(send_pcb, request, &PUZZLE_SERVER_IP, PUZZLE_SERVER_PORT);
	pbuf_free(request);
}

void browse_solutions(field* fields, int num_fields) {
	int cur_field = 0;
	char next_instr[2] = " ";

	while (1) {
		switch (next_instr[0]) {
		case 'n':
			cur_field = (cur_field + 1) % num_fields;
			break;
		case 'p':
			if (cur_field == 0) {
				cur_field = num_fields;
			}
			cur_field -= 1;
			break;
		case 'q':
			return;
		}
		printf("\nSolution %d:\n", cur_field + 1);
		print_field(&fields[cur_field]);
		printf("\n");
    	show_field_hdmi(&fields[cur_field]);

		printf("What would you like to do next? [(p)revious, (n)ext, (q)uit]");
		scanf("%2s", next_instr);
	}
}

void udp_get_handler(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    // Check that a valid protocol control block was received
    if (p) {
    	char* payload = (char*)p->payload;

    	printf("\nGot a packet:\n");
    	char header[MSG_HEADER_SIZE];
        memcpy(header, payload, MSG_HEADER_SIZE);
        printf("Header: [%d, %d, %d, %d, %d, %d]\n", header[0], header[1], header[2], header[3], header[4], header[5]);
        unsigned char size = header[1];
        uint32_t seed = seed_from_bytes(&header[2]);
        printf("Header spec: size=%d, seed=%ld\n", size, seed);

        unsigned int tile_bytes = size * size * 4;
        char body[tile_bytes];
        memcpy(body, &payload[MSG_HEADER_SIZE], tile_bytes);

        int num_tiles = tile_bytes / 4;
        printf("Followed by %d bytes, forming %d tiles\n", tile_bytes, num_tiles);

        // Assemble the field
        field f = new_field(size);
        for (int t = 0; t < num_tiles; t++) {
        	for (int s = 0; s < 4; s++) {
        		int body_off = (t * 4) + s;
        		f.tiles[t].colours[s] = body[body_off];
        	}
        }

        // Solve it
        printf("\nTrying to solve...\n");
        field solved[MAX_SOLVED];
        int num_solved = solve(solved, &f);

        if (!num_solved) {
        	printf("Couldn't find any solutions	\n");
        } else {
        	printf("Found %d solutions, opening browser\n", num_solved);
        	browse_solutions(solved, num_solved);
        }

        pbuf_free(p);

        // Send another request!
        printf("\n");
		make_and_send_puzzle_req(pcb);
    } else {
    	printf("ERROR: invalid protocol control block received\n");
    }
}


int main()
{
	IP4_ADDR(&PUZZLE_SERVER_IP, 192, 168, 10, 1);

    print("\n\n\n\n================================================================\n\r");

	hdmi_setup();

    printf("\nAs a fallback, you may opt not to use ethernet and show only a preset solution over HDMI.\n");
    printf("Enter 1 to continue with eth setup as normal, or 2 to just show HDMI\n");

    char choice[2];
    scanf("%2s", choice);

    if (choice[0] == '2') {
    	printf("HDMI static mode selected\n");

    	const tile SAMPLE_TILES[16] = {
			{{3, 1, 4, 6}, 0},
			{{8, 7, 3, 3}, 0},
			{{4, 5, 8, 9}, 0},
			{{2, 1, 4, 1}, 0},
			{{8, 4, 3, 1}, 0},
			{{2, 6, 8, 7}, 0},
			{{7, 1, 2, 5}, 0},
			{{2, 3, 7, 1}, 0},
			{{5, 1, 9, 4}, 0},
			{{6, 1, 5, 6}, 0},
			{{9, 3, 6, 1}, 0},
			{{1, 2, 9, 3}, 0},
			{{6, 7, 2, 1}, 0},
			{{5, 8, 6, 1}, 0},
			{{5, 2, 5, 3}, 0},
			{{4, 5, 5, 2}, 0}
    	};

    	field sample = new_field(4);
    	sample.tiles = SAMPLE_TILES;

    	for (int i = 0; i < 16; i++) {
    		sample.placed[i] = 1;
    		sample.inner[i] = i;
    	}

    	printf("Printing this pre-solved field to HDMI:\n");
    	print_field(&sample);
    	show_field_hdmi(&sample);

    	while (1) {}
    }

	init_platform(MAC_ADDR, NULL, NULL);

	struct udp_pcb *recv_pcb = udp_new();
	if (!recv_pcb) {
		printf("Error creating PCB\n");
	}
	// Listen on port 7001
	udp_bind(recv_pcb, IP_ADDR_ANY, PUZZLE_SERVER_PORT);
	// Set up the receive handler
	udp_recv(recv_pcb, udp_get_handler, NULL);

	printf("Setup Complete\n\n");

	struct udp_pcb *send_pcb = udp_new();
	make_and_send_puzzle_req(send_pcb);
	// Remove the send PCB because we use the recv pcb in the handler
	udp_remove(send_pcb);

	printf("\n\n");
	// Now enter the handling loop
	while (1) {
		handle_ethernet();
	}

    return 0;
}
