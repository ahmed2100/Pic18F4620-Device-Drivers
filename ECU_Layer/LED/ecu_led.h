/* 
 * File:   ecu_led.h
 * Author: ahmed
 *
 * Created on July 11, 2022, 11:25 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H



/* Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_led_config.h"

/* Macro Defines */

/* Macro Functions */

/* User-Defined Data Type Declaration */

typedef enum {
    LED_OFF,
    LED_ON
} led_status ;

typedef struct {
    uint8_t port_name : 4;
    uint8_t pin : 3;
    uint8_t led_status : 1;

}led_t ;


/* Software Interfaces (Prototypes) */

Std_ReturnType ecu_led_init(const led_t *led);
Std_ReturnType ecu_led_turn_on(led_t *led);
Std_ReturnType ecu_led_turn_off(led_t *led);
Std_ReturnType ecu_led_turn_toggle(led_t *led);


/* Global Shared Variables */

#endif	/* ECU_LED_H */

