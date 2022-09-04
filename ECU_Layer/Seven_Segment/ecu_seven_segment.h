/* 
 * File:   ecu_seven_segment.h
 * Author: ahmed
 *
 * Created on July 25, 2022, 1:41 PM
 */

#ifndef ECU_SEVEN_SEGMENT_H
#define	ECU_SEVEN_SEGMENT_H

/* Includes */
#include "ecu_seven_segment_config.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Macro Defines */
#define SEGMENT_PIN0    0
#define SEGMENT_PIN1    1
#define SEGMENT_PIN2    2
#define SEGMENT_PIN3    3


/* Macro Functions */

/* User-Defined Data Type Declaration */

typedef enum {
    SEGMENT_COMMON_CATHODE = 0,
    SEGMENT_COMMON_ANODE
}segment_type_t;

typedef struct {
    pin_config_t segment_pins[4];
    segment_type_t segment_type;
}segment_t;

/* Software Interfaces (Prototypes) */

Std_ReturnType ecu_seven_segment_init(const segment_t *segment);
Std_ReturnType ecu_seven_segment_write_number(const segment_t *segment , uint8_t number);


/* Global Shared Variables */




#endif	/* ECU_SEVEN_SEGMENT_H */

