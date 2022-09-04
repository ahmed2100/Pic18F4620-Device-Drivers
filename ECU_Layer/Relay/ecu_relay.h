/* 
 * File:   ecu_relay.h
 * Author: ahmed
 *
 * Created on July 22, 2022, 10:27 AM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H


/* Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_relay_config.h"


/* Macro Defines */

/* Macro Functions */

/* User-Defined Data Type Declaration */
typedef enum 
{
    RELAY_OFF = 0 ,
    RELAY_ON
} relay_state_t ;

typedef struct 
{
    uint8_t port : 4 ;
    uint8_t pin : 3 ;
    uint8_t state : 1 ;
} relay_t;


/* Software Interfaces (Prototypes) */
Std_ReturnType ecu_relay_init(const relay_t *relay);
Std_ReturnType ecu_relay_turn_on(const relay_t *relay);
Std_ReturnType ecu_relay_turn_off(const relay_t *relay);


/* Global Shared Variables */



#endif	/* ECU_RELAY_H */

