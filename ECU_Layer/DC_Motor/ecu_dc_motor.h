/* 
 * File:   ecu_dc_motor.h
 * Author: ahmed
 *
 * Created on July 23, 2022, 8:34 AM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H

/* Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_dc_motor_config.h"


/* Macro Defines */

#define DC_MOTOR_RIGHT_PIN 0x00U
#define DC_MOTOR_LEFT_PIN  0x01U

/* Macro Functions */

/* User-Defined Data Type Declaration */

typedef enum {
    DC_MOTOR_PIN_OFF = 0,
    DC_MOTOR_PIN_ON
}dc_motor_pin_state_t ;



// typedef struct {
//     dc_motor_pin_t forward_pin ;
//     dc_motor_pin_t reverse_pin ;

// }dc_motor_t ;

typedef struct {
    pin_config_t dc_motor[2]; // array of two pins

} dc_motor_t ;


/* Software Interfaces (Prototypes) */

Std_ReturnType ecu_dc_motor_init(const dc_motor_t *dc_motor);
Std_ReturnType ecu_dc_motor_run_left(const dc_motor_t *dc_motor);
Std_ReturnType ecu_dc_motor_run_right(const dc_motor_t *dc_motor);
Std_ReturnType ecu_dc_motor_stop(const dc_motor_t *dc_motor);


/* Global Shared Variables */


#endif	/* ECU_DC_MOTOR_H */

