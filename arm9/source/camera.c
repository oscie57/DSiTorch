#include "camera.h"

#include <nds.h>

// https://problemkaputt.de/gbatek-dsi-cameras.htm

Camera activeCamera    = CAM_NONE;
CaptureMode activeMode = CAPTURE_MODE_PREVIEW;

bool cameraInit() {
	REG_SCFG_CLK |= BIT(2); // CamInterfaceClock = ON
	REG_CAM_MCNT = 0;       // Camera Module Control
	swiDelay(0x1E);
	REG_SCFG_CLK |= BIT(8); // CamExternal Clock = ON
	swiDelay(0x1E);
	REG_CAM_MCNT = BIT(1) | BIT(5); // Camera Module Control
	swiDelay(0x2008);
	REG_SCFG_CLK &= ~BIT(8); // CamExternal Clock = OFF
	REG_CAM_CNT &= ~BIT(15); // allow changing params
	REG_CAM_CNT |= BIT(5);   // flush data fifo
	REG_CAM_CNT = (REG_CAM_CNT & ~(0x0300)) | 0x0200;
	REG_CAM_CNT |= BIT(10);
	REG_CAM_CNT |= BIT(11); // irq enable
	REG_SCFG_CLK |= BIT(8); // CamExternal Clock = ON
	swiDelay(0x14);

	// issue "aptina_code_list_init" via I2C bus on ARM7 side
	fifoSendValue32(FIFO_CAMERA, CAM_INIT);
	fifoWaitValue32(FIFO_CAMERA);
	u32 val = fifoGetValue32(FIFO_CAMERA);

	REG_SCFG_CLK &= ~BIT(8); // CamExternal Clock = OFF
	REG_SCFG_CLK |= BIT(8);  // CamExternal Clock = ON
	swiDelay(0x14);

	return val == 0x2280;
}

bool cameraActivate(Camera cam) {
	if(activeCamera != CAM_NONE)
		cameraDeactivate(activeCamera);

	u32 command = (cam == CAM_INNER) ? CAM0_ACTIVATE : CAM1_ACTIVATE;
	fifoSendValue32(FIFO_CAMERA, command);
	fifoWaitValue32(FIFO_CAMERA);

	if(fifoGetValue32(FIFO_CAMERA) == command) {
		activeCamera = cam;
		return true;
	} else {
		return false;
	}
}

bool cameraDeactivate(Camera cam) {
	u32 command = (cam == CAM_INNER) ? CAM0_DEACTIVATE : CAM1_DEACTIVATE;
	fifoSendValue32(FIFO_CAMERA, command);
	fifoWaitValue32(FIFO_CAMERA);

	if(fifoGetValue32(FIFO_CAMERA) == command) {
		activeCamera = CAM_NONE;
		return true;
	} else {
		return false;
	}
}
