#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xil_cache.h"

void udp_get_handler(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port) {
    // Check that a valid protocol control block was received
    if (p) {
        // The message may not be zero terminated, so to ensure that we only
        // print what was sent, we can create a zero-terminated copy and print that.
        char msg[p->len + 1];
        memcpy(msg, p->payload, p->len);
        msg[p->len] = '\0';

        printf("Message: %s\n", msg);

        // This is how we would send a reply back to the address which messaged us on port 7000
        //udp_sendto(pcb, p, addr, 7000);

        // Don't forget to free the packet buffer!
        pbuf_free(p);
    }
}

int main()
{
	unsigned char mac_ethernet_address[] = {0x00, 0x11, 0x22, 0x33, 0x00, 0xXX}; // Put your own MAC address here!
	init_platform(mac_ethernet_address, NULL, NULL);

    print("Hello World\n\r");
    print("Successfully ran Hello World application");
    return 0;
}
