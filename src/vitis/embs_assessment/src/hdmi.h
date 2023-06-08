#ifndef MY_HDMI
#define MY_HDMI

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

const int TILE_SIDE = 90;

DisplayCtrl dispCtrl; // Display driver struct
u32 frameBuf[DISPLAY_NUM_FRAMES][MAX_FRAME] __attribute__((aligned(0x20))); // Frame buffers for video data
void *pFrames[DISPLAY_NUM_FRAMES]; // Array of pointers to the frame buffers

void hdmi_setup(void);

void show_field_hdmi(field* f);

#endif
