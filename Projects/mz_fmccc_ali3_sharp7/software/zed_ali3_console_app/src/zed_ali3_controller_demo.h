//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
//----------------------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to:  technical.support@avnet.com
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2013 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Jul 01, 2013
// Design Name:         ZedBoard ALI3 Controller Demonstration
// Module Name:         zed_ali3_controller_demo.h
// Project Name:        ZedBoard ALI3 Display Kit
// Target Devices:      Zynq-7000
// Hardware Boards:     ZedBoard, MicroZed, Zed Display Kit
//
// Tool versions:       Vivado 2015.2
//
// Description:         Zed Display Kit Demo
//                      This application will configure the display kit for
//                      480*800 display output.
//
// Dependencies:
//
// Revision:            Nov 15, 2011: 1.00 Initial version
//                      May 23, 2013: 1.02 Updated for Zed Display Kit - 14.5
//                      Jul  1, 2013: 1.03 Revised for MicroZed target
//                      Feb  7, 2014: 1.03 Revised for MicroZed FMC-CC target
//                      Sep 17, 2015: 1.04 Updated for 2015.2 release
//
//----------------------------------------------------------------------------

#ifndef __ZED_ALI3_CONTROLLER_DEMO_H__
#define __ZED_ALI3_CONTROLLER_DEMO_H__

#include "xparameters.h"
#include "types.h"

#include "sleep.h"

// I2C related.
#include "zed_iic.h"

// Video related
#include "xaxivdma.h"
#include "video_resolution.h"
#include "video_frame_buffer.h"

// Interrupt related.
#include "xscugic.h"
#include "xil_exception.h"

// PS GPIO related.
#include "xgpiops.h"

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID

/* User interface positional map. */
#define BUTTON0_POSITION_X                     321 // "BTN1"
#define BUTTON1_POSITION_X                     412 // "BTN2"
#define BUTTON0_POSITION_Y                     190 // "BTN1"
#define BUTTON1_POSITION_Y                     190 // "BTN2"

#define LED0_POSITION_X                        230 // "LED1"
#define LED1_POSITION_X                        321 // "LED2"
#define LED2_POSITION_X                        412 // "LED3"
#define LED3_POSITION_X                        503 // "LED4"
#define LED0_POSITION_Y                        270 // "LED1"
#define LED1_POSITION_Y                        270 // "LED2"
#define LED2_POSITION_Y                        270 // "LED3"
#define LED3_POSITION_Y                        270 // "LED4"

/* Defines a demo mode enumeration type. */
enum demo_mode
{
    draw = 0,
    control
};

/* Defines a touch controller enumeration type.  The enumeration values
 * just happen to also be the I2C slave address values. */
enum touch_controller_type
{
    none = 0,
    clicktouch = 0x1F,
    dhelectronic = 0x20
};

/* This structure contains the context for the Zed Display Kit
 * Standalone Demonstration Application. */
struct struct_zed_ali3_controller_demo_t
{
    int32u bVerbose;

    ////////////////////////////////
    // Video DMA related context
    ////////////////////////////////
    int32u uDeviceId_VDMA_FrameBuffer;
    int32u uBaseAddr_MEM_FrameBuffer;  // address of FB in memory
    int32u uNumFrames_FrameBuffer;

    XAxiVdma vdma_ali3;
    XAxiVdma_DmaSetup vdmacfg_ali3_read;
    XAxiVdma_DmaSetup vdmacfg_ali3_write; // not used in this demo

    // ALI3 video resolution
    int32u ali3_width;
    int32u ali3_height;
    int32u ali3_resolution;

    ////////////////////////////////
    // Touch I2C related context
    ////////////////////////////////
    int32u uBaseAddr_IIC_Touch;

    zed_iic_t touch_iic;

    ////////////////////////////////
    // Interrupt related context
    ////////////////////////////////
    int32u uDeviceId_IRQ_Touch;
    int32u uInterruptId_IRQ_Touch;

    XScuGic Intc;
    XScuGic_Config *pIntcConfig;

    ////////////////////////////////
    // Touch calibration coefficients
    ////////////////////////////////
    int32s calibration_success;
    int32s calibration_An;
    int32s calibration_Bn;
    int32s calibration_Cn;
    int32s calibration_Dn;
    int32s calibration_En;
    int32s calibration_Fn;
    int32s calibration_divisor;

    ////////////////////////////////
    // Touch related statistics
    ////////////////////////////////
    int32u touch_duplicates;
    int32u touch_events;
    int32u touch_irqs;
    int32u touch_overflows;
    int16u touch_posx_raw;
    int16u touch_posy_raw;
    int16u touch_posx_cal;
    int16u touch_posy_cal;
    int8u  touch_gesture;
    int8u  touch_pen_down;
    int8u  touch_pen_down_transition;
    int32u touch_valid;

    ////////////////////////////////
	// Touch I2C slave related context
	////////////////////////////////
	int8u slave_touch_data_offset;
	int8u slave_touch_report_length;
	int8u touch_max_finger_count_supported;

	enum touch_controller_type controller_type;

    ////////////////////////////////
    // GPIO related members
    ////////////////////////////////
    int button0_state;
    int button1_state;

    int led0_state;
    int led1_state;
    int led2_state;
    int led3_state;

    XGpioPs gpio_driver;	// The driver instance for PS GPIO Device.

    enum demo_mode mode;
};
typedef struct struct_zed_ali3_controller_demo_t zed_ali3_controller_demo_t;

int zed_ali3_controller_demo_board_process(zed_ali3_controller_demo_t *pDemo);
int zed_ali3_controller_demo_button(zed_ali3_controller_demo_t *pDemo, int button_number, int button_state);
int zed_ali3_controller_demo_init(zed_ali3_controller_demo_t *pDemo);
int zed_ali3_controller_demo_cbars(zed_ali3_controller_demo_t *pDemo, int32u offset);
int zed_ali3_controller_demo_check_calibrate_request(zed_ali3_controller_demo_t *pDemo);
int zed_ali3_controller_demo_control(zed_ali3_controller_demo_t *pDemo);
int zed_ali3_controller_demo_led(zed_ali3_controller_demo_t *pDemo, int led_number, int led_state);
int zed_ali3_controller_demo_logo(zed_ali3_controller_demo_t *pDemo);
int zed_ali3_controller_demo_touch_calibrate(zed_ali3_controller_demo_t *pDemo);
int zed_ali3_controller_demo_touch_process(zed_ali3_controller_demo_t *pDemo);

#endif // __ZED_ALI3_CONTROLLER_DEMO_H__
