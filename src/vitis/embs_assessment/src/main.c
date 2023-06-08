#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include "field.h"

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

void make_puzzle_req(char* buf) {
	// Assemble the request
//	printf("Enter puzzle size: ");
//	char size;
//	scanf("%hhu\n", &size);
//	printf("Enter random seed [0]: ");
//	uint32_t seed;
//	scanf("%lu\n");

	char size = 2;
	uint32_t seed = 0;

	buf[0] = 1;
	buf[1] = size;
	bytes_from_seed(&buf[2], seed);
}

void make_and_send_puzzle_req(struct udp_pcb *send_pcb) {
	// Send a request for the puzzle
	char request_msg[6];
	make_puzzle_req(request_msg);
	// Create a packet buffer and set the payload as the message
	printf("Sending [%d, %d, %d, %d, %d, %d]\n", request_msg[0], request_msg[1], request_msg[2], request_msg[3], request_msg[4], request_msg[5]);
    struct pbuf * request = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_REF);
    request->payload = request_msg;
	// Send the message
	udp_sendto(send_pcb, request, &PUZZLE_SERVER_IP, PUZZLE_SERVER_PORT);
	pbuf_free(request);
}

void udp_get_handler(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    // Check that a valid protocol control block was received
    if (p) {
    	char* payload = (char*)p->payload;

    	printf("\n\nGot a new packet:\n");
    	char header[MSG_HEADER_SIZE];
        memcpy(header, payload, MSG_HEADER_SIZE);
        printf("Header: [%d, %d, %d, %d, %d, %d]\n", header[0], header[1], header[2], header[3], header[4], header[5]);
        unsigned char size = header[1];
        uint32_t seed = seed_from_bytes(&header[2]);
        printf("Header spec: size=%d\n", size);

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
        place(&f, 0, c(0, 0));
        if (!solve(&f)) {
        	printf("Couldn't find any solutions, I tried this start:\n");
        } else {
        	printf("Found a solution:\n");
        }
        print_field(&f);

        pbuf_free(p);

        // Send another request!
    } else {
    	printf("ERROR: invalid protocol control block received\n");
    }
}


int main()
{
	IP4_ADDR(&PUZZLE_SERVER_IP, 192, 168, 10, 1);

    print("\n\n\n\n================================================================\n\rHello World\n\r");

	init_platform(MAC_ADDR, NULL, NULL);

	struct udp_pcb *recv_pcb = udp_new();
	if (!recv_pcb) {
		printf("Error creating PCB\n");
	}
	// Listen on port 7001
	udp_bind(recv_pcb, IP_ADDR_ANY, PUZZLE_SERVER_PORT);
	// Set up the receive handler
	udp_recv(recv_pcb, udp_get_handler, NULL);
	// Create a protocol control block (PCB)
	struct udp_pcb *send_pcb = udp_new();

	// Send a request for the puzzle
	char request_msg[6];
	make_puzzle_req(request_msg);
	// Create a packet buffer and set the payload as the message
	printf("Sending [%d, %d, %d, %d, %d, %d]\n", request_msg[0], request_msg[1], request_msg[2], request_msg[3], request_msg[4], request_msg[5]);
    struct pbuf * request = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_REF);
    request->payload = request_msg;
	// Send the message
	udp_sendto(send_pcb, request, &PUZZLE_SERVER_IP, PUZZLE_SERVER_PORT);
	pbuf_free(request);

	// Remove the send PCB because we don't re-use it in this example
	udp_remove(send_pcb);

	// Now enter the handling loop
	while (1) {
		handle_ethernet();
	}

    return 0;
}
