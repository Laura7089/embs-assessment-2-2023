#include <stdio.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "zybo_z7_hdmi/display_ctrl.h"
#include "field.h"

uint32_t colour_mapping[] = {
	0xd74385,
	0x6fbb40,
	0xc34acf,
	0x4fab82,
	0x6e5cd4,
	0xcb9443,
	0x678acc,
	0xd34e30,
	0xb56eb1,
	0x7f883b,
	0xc16264
};

// Frame size (based on 1440x900 resolution, 32 bits per pixel)
#define MAX_FRAME (1440*900)
#define FRAME_STRIDE (1440*4)

const int COLOUR_SIDE = 30;
const int TILE_SIDE = 3 * COLOUR_SIDE;

DisplayCtrl dispCtrl; // Display driver struct
u32 frameBuf[DISPLAY_NUM_FRAMES][MAX_FRAME] __attribute__((aligned(0x20))); // Frame buffers for video data
void *pFrames[DISPLAY_NUM_FRAMES]; // Array of pointers to the frame buffers

void hdmi_setup(void);

void show_field_hdmi(field* f);

void hdmi_setup(void) {
	// Initialise an array of pointers to the 2 frame buffers
	int i;
	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
		pFrames[i] = frameBuf[i];

	// Initialise the display controller
	DisplayInitialize(&dispCtrl, XPAR_AXIVDMA_0_DEVICE_ID, XPAR_VTC_0_DEVICE_ID, XPAR_HDMI_AXI_DYNCLK_0_BASEADDR, pFrames, FRAME_STRIDE);

	// Use first frame buffer (of two)
	DisplayChangeFrame(&dispCtrl, 0);

	// Set the display resolution
	DisplaySetMode(&dispCtrl, &VMODE_1440x900);

	// Enable video output
	DisplayStart(&dispCtrl);

	printf("\n\r");
	printf("HDMI output enabled\n\r");
	printf("Current Resolution: %s\n\r", dispCtrl.vMode.label);
	printf("Pixel Clock Frequency: %.3fMHz\n\r", dispCtrl.pxlFreq);
}

void show_gradient(void) {
	// Get parameters from display controller struct
	int x, y;
	u32 stride = dispCtrl.stride / 4;
	u32 width = dispCtrl.vMode.width;
	u32 height = dispCtrl.vMode.height;
	u32 *frame = (u32 *)dispCtrl.framePtr[dispCtrl.curFrame];
	u32 red, green, blue;

	// Fill the screen with a nice gradient pattern
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			green = (x*0xFF) / width;
			blue = 0xFF - ((x*0xFF) / width);
			red = (y*0xFF) / height;
			frame[y*stride + x] = (red << BIT_DISPLAY_RED) | (green << BIT_DISPLAY_GREEN) | (blue << BIT_DISPLAY_BLUE);
		}
	}

	// Flush the cache, so the Video DMA core can pick up our frame buffer changes.
	// Flushing the entire cache (rather than a subset of cache lines) makes sense as our buffer is so big
	Xil_DCacheFlush();
	printf("Output written to screen\n\r");
}

void write_colour_row(u32* buf, u32 col) {
	for (int i = 0; i < TILE_SIDE; i++) {
		buf[i] = col;
	}
}

void show_field_hdmi(field* f) {
	// Get parameters from display controller struct
	u32 stride = dispCtrl.stride / 4;
	u32 width = dispCtrl.vMode.width;
	u32 height = dispCtrl.vMode.height;
	u32 *frame = (u32 *)dispCtrl.framePtr[dispCtrl.curFrame];

	// Clear the Screen
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			frame[y*stride + x] = 0;
		}
	}

	// Flush the cache, so the Video DMA core can pick up our frame buffer changes.
	// Flushing the entire cache (rather than a subset of cache lines) makes sense as our buffer is so big
	Xil_DCacheFlush();

	for (int tile_y = f->size-1; tile_y >= 0; tile_y--) {
		int num_lines = (f->size - 1 - tile_y) * TILE_SIDE;
		int buf_offset = num_lines * stride;

		// Top Row
		for (int _row = 0; _row < COLOUR_SIDE; _row++) {
			int this_row_off = buf_offset;
			for (int tile_xt = 0; tile_xt < f->size; tile_xt++) {
				int t = idx(f, c(tile_xt, tile_y));
				if (t != f->num_tiles) {
					int colour = colour_mapping[get_side(f->tiles[t], top)];
					// Blank
					write_colour_row(&frame[this_row_off], 0);
					this_row_off += COLOUR_SIDE;

					write_colour_row(&frame[this_row_off], colour);
					this_row_off += COLOUR_SIDE;

					// Blank
					write_colour_row(&frame[this_row_off], 0);
					this_row_off += COLOUR_SIDE;
				} else {
					// Do nothing if the tile isn't placed, leave the space black
					this_row_off += TILE_SIDE;
				}
			}
			buf_offset += stride;
		}
		// Middle Row
		for (int _row = 0; _row < COLOUR_SIDE; _row++) {
			int this_row_off = buf_offset;
			for (int tile_xt = 0; tile_xt < f->size; tile_xt++) {
				int t = idx(f, c(tile_xt, tile_y));
				if (t != f->num_tiles) {
					int cl = colour_mapping[get_side(f->tiles[t], left)];
					int cr = colour_mapping[get_side(f->tiles[t], right)];

					write_colour_row(&frame[this_row_off], cl);
					this_row_off += COLOUR_SIDE;

					// Blank
					write_colour_row(&frame[this_row_off], 0);
					this_row_off += COLOUR_SIDE;

					write_colour_row(&frame[this_row_off], cr);
					this_row_off += COLOUR_SIDE;
				} else {
					// Do nothing if the tile isn't placed, leave the space black
					this_row_off += TILE_SIDE;
				}
			}
			buf_offset += stride;
		}
		// Bottom Row
		for (int _row = 0; _row < COLOUR_SIDE; _row++) {
			int this_row_off = buf_offset;
			for (int tile_xt = 0; tile_xt < f->size; tile_xt++) {
				int t = idx(f, c(tile_xt, tile_y));
				if (t != f->num_tiles) {
					int colour = colour_mapping[get_side(f->tiles[t], bottom)];
					// Blank
					write_colour_row(&frame[this_row_off], 0);
					this_row_off += COLOUR_SIDE;

					write_colour_row(&frame[this_row_off], colour);
					this_row_off += COLOUR_SIDE;

					// Blank
					write_colour_row(&frame[this_row_off], 0);
					this_row_off += COLOUR_SIDE;
				} else {
					// Do nothing if the tile isn't placed, leave the space black
					this_row_off += TILE_SIDE;
				}
			}
			buf_offset += stride;
		}
	}

	Xil_DCacheFlush();

	printf("Output written to screen\n\r");
}
