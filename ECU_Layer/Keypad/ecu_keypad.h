/* 
 * File:   ecu_keypad.h
 * Author: ahmed
 *
 * Created on July 26, 2022, 7:16 AM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H


/* Includes */

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_keypad_config.h"

/* Macro Defines */

#define NUMBER_OF_ROWS      4
#define NUMBER_OF_COLS      4

/* Macro Functions */

/* User-Defined Data Type Declaration */
typedef struct {
    pin_config_t row_pins[NUMBER_OF_ROWS];
    pin_config_t columns_pins[NUMBER_OF_COLS];
}keypad_t;

/* Software Interfaces (Prototypes) */

Std_ReturnType ecu_keypad_init(const keypad_t *keypad);
Std_ReturnType ecu_keypad_get_value(const keypad_t *keypad , uint8_t *value);

/* Global Shared Variables */



#endif	/* ECU_KEYPAD_H */

