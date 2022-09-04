/* 
 * File:   ecu_init.h
 * Author: ahmed
 *
 * Created on July 26, 2022, 8:41 AM
 */

#ifndef ECU_INIT_H
#define	ECU_INIT_H

/* Includes */

#include "LED/ecu_led.h"
#include "Button/ecu_push_button.h"
#include "Relay/ecu_relay.h"
#include "DC_Motor/ecu_dc_motor.h"
#include "Seven_Segment/ecu_seven_segment.h"
#include "Keypad/ecu_keypad.h"
#include "Chr_LCD/ecu_chr_lcd.h"

/* Macro Defines */

/* Macro Functions */

/* User-Defined Data Type Declaration */

/* Software Interfaces (Prototypes) */
void ecu_init(void);

/* Global Shared Variables */
extern btn_t btn1;
extern led_t led1;
extern led_t led2;


#endif	/* ECU_INIT_H */

