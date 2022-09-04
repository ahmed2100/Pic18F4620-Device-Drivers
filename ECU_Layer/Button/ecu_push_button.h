/* 
 * File:   ecu_push_button.h
 * Author: ahmed
 *
 * Created on July 11, 2022, 11:08 PM
 */

#ifndef ECU_PUSH_BUTTON_H
#define ECU_PUSH_BUTTON_H
 
/* Includes */

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_push_button_config.h"


/* Macro Defines */

/* Macro Functions */

/* User-Defined Data Type Declaration */



typedef enum 
{
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
}btn_state_t;

typedef enum
{
    BUTTON_ACTIVE_LOW = 0,
    BUTTON_ACTIVE_HIGH
}btn_connection_type_t ;

typedef struct 
{
    uint8_t port : 3;
    uint8_t pin : 3;
    uint8_t state : 1;
    uint8_t connection : 1;

}btn_t;

/* Software Interfaces (Prototypes) */

Std_ReturnType ecu_btn_init(const btn_t *btn);
Std_ReturnType ecu_btn_read_logic(const btn_t *btn , btn_state_t *btn_state);


/* Global Shared Variables */


#endif