#include <nds.h>
#include <stdio.h>

#include "camera.h"

#include "bottom0.h"
#include "bottom1.h"
#include "top0.h"
#include "top1.h"

bool status = false;

void turnOn() {
	cameraActivate( CAM_OUTER );
	// set backgrounds to top1 and bottom1
	status = true;
}

void turnOff() {
	cameraDeactivate( CAM_OUTER );
	// set backgrounds to top0 and bottom0
	status = false;
}

int main(void) {
	touchPosition touch;

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("Initializing Camera...");
	cameraInit();
	iprintf("Camera Initialized!");

	iprintf("\x1b[2J");

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\twww.devkitpro.org");

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
