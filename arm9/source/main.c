#include <nds.h>
#include <stdio.h>

#include "camera.h"

#include "bottom0.h"
#include "bottom1.h"
#include "top0.h"
#include "top1.h"

bool status = false;

void turnOn() {
	printf("writing - torch on");
	fifoSendValue32(FIFO_CAMERA, FLASH_ACTIVATE); // turns on torch
	fifoWaitValue32(FIFO_CAMERA);
	fifoGetValue32(FIFO_CAMERA);
	printf("written - torch on");
	// set backgrounds to top1 and bottom1
	status = true;
}

void turnOff() {
	printf("writing - torch off");
	fifoSendValue32(FIFO_CAMERA, FLASH_DEACTIVATE);  // turns off torch
	fifoWaitValue32(FIFO_CAMERA);
	fifoGetValue32(FIFO_CAMERA);
	printf("written - torch off");
	// set backgrounds to top0 and bottom0
	status = false;
}

int main(void) {
	touchPosition touch;

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("\x1b[2J");

	while(1) {

		touchRead(&touch);
		iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
		iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);

		status = false;
		// set backgrounds to top0 and bottom0

		swiWaitForVBlank();
		scanKeys();

		if ( keysDown() & KEY_START ) {
			break;
		};
		if ( keysDown() & KEY_A ) {

			// play sound effect

			if (status == false) {
				iprintf("Turning On. ");
				turnOn();
			};
			if (status == true) {
				iprintf("Turning Off. ");
				turnOff();
			};

		};
	}

	return 0;
}
