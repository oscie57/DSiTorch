#include <nds.h>
#include <stdio.h>

#include "camera.h"

#include "bottom0.h"
#include "bottom1.h"
#include "top0.h"
#include "top1.h"

bool status = false;

int bgTop;
int bgBot;


void turnOn() {
	// turn the torch on
	fifoSendValue32(FIFO_CAMERA, FLASH_ACTIVATE); // turns on torch
	fifoWaitValue32(FIFO_CAMERA);
	fifoGetValue32(FIFO_CAMERA);

	// set backgrounds to top1 and bottom1
	dmaCopy(top1Bitmap, bgGetGfxPtr(bgTop), 256*256);
	dmaCopy(top1Pal, BG_PALETTE, 256*2);
	dmaCopy(bottom1Bitmap, bgGetGfxPtr(bgBot), 256*256);
	dmaCopy(bottom1Pal, BG_PALETTE_SUB, 256*2);

	// set status to on
	status = true;
}

void turnOff() {
	// turn the torch off
	fifoSendValue32(FIFO_CAMERA, FLASH_DEACTIVATE);
	fifoWaitValue32(FIFO_CAMERA);
	fifoGetValue32(FIFO_CAMERA);

	// set backgrounds to top0 and bottom0
	dmaCopy(top0Bitmap, bgGetGfxPtr(bgTop), 256*256);
	dmaCopy(top0Pal, BG_PALETTE, 256*2);
	dmaCopy(bottom0Bitmap, bgGetGfxPtr(bgBot), 256*256);
	dmaCopy(bottom0Pal, BG_PALETTE_SUB, 256*2);

	// set status to false
	status = false;
}

int main(void) {
	touchPosition touch;

	vramSetBankA(VRAM_A_MAIN_BG); // for the top
	vramSetBankC(VRAM_C_SUB_BG); // for the bottom

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	iprintf("\x1b[2J");

	status = false;

	// consoleDemoInit();  //setup the sub screen for printing

	// set backgrounds to top0 and bottom0
	bgTop = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	bgBot = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(top0Bitmap, bgGetGfxPtr(bgTop), 256*256);
	dmaCopy(top0Pal, BG_PALETTE, 256*2);
	dmaCopy(bottom0Bitmap, bgGetGfxPtr(bgBot), 256*256);
	dmaCopy(bottom0Pal, BG_PALETTE_SUB, 256*2);

	while(1) {

		touchRead(&touch);
		iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		swiWaitForVBlank();
		scanKeys();

		if ( keysDown() & KEY_START ) {
			break;
		}

		if ( keysDown() & KEY_A ) {

			// play sound effect

			if (status == false) {
				iprintf("Turning On. ");
				turnOn();
			} else if (status == true) {
				iprintf("Turning Off. ");
				turnOff();
			}
		}
	}

	return 0;
}
