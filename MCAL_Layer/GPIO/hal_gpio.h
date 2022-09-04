/* 
 * File:   hal_gpio.h
 * Author: ahmed
 *
 * Created on July 11, 2022, 11:08 PM
 */

#ifndef HAL_GPIO_H 
#define	HAL_GPIO_H

/* Includes */
#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "hal_gpio_config.h"


/* Macro Defines */


/* Port Macros for input output config */
#define GPIO_PORT_ALL_OUT               0x00
#define GPIO_PORT_ALL_IN                0xFF



#define BIT_MASK           (uint8_t) 1
#define PORT_MAX_NUMBER    5
#define PIN_MAX_NUMBER     8

#define GPIO_PIN_CONFIG     CONFIG_ENABLE
#define GPIO_PORT_CONFIG    CONFIG_ENABLE



/* Macro Functions */

#define HWREG8(_x)                   (*((volatile uint8_t *)_x))
#define SET_BIT(REG,BIT_POSN)        ( REG |=  (BIT_MASK << BIT_POSN) )
#define CLEAR_BIT(REG,BIT_POSN)      ( REG &= ~(BIT_MASK << BIT_POSN) )
#define TOGGLE_BIT(REG,BIT_POSN)     ( REG ^=  (BIT_MASK << BIT_POSN) )
#define READ_BIT(REG , BIT_POSN)     ( (REG >> BIT_POSN) & BIT_MASK )
#define PORT_ALL_SET_MASK          0xFF

/* User-Defined Data Type Declaration */
typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH
}logic_t;

typedef enum
{
    OUTPUT = 0,
    INPUT

}direction_t;

typedef enum 
{
    PIN0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7
}pin_index_t;

typedef enum 
{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX
}port_index_t;



typedef struct 
{
    uint8_t port : 3;       /* @ref port_index_t */
    uint8_t pin : 3;        /* @ref pin_index_t */
    uint8_t direction : 1;  /* @ref direction_t */
    uint8_t logic : 1;      /* @ref logic_t */
}pin_config_t ;


/* Software Interfaces (Prototypes) */

/* Pin Configuration */

Std_ReturnType gpio_pin_direction_init(const pin_config_t *_pin_config);

//Std_ReturnType gpio_pin_get_direction_status(pin_config_t *_pin_config , uint8_t *_pin_dir_status); // one idea

Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config , direction_t *dir_status); // another idea

Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config , logic_t logic);

Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config , logic_t *logic);

Std_ReturnType gpio_pin_toggle_logic(const pin_config_t *_pin_config);

Std_ReturnType gpio_pin_init(const pin_config_t *_pin_config);


/* Port Configuration */

Std_ReturnType gpio_port_direction_init(port_index_t port , uint8_t dir_status);
Std_ReturnType gpio_port_get_direction_status(port_index_t port , uint8_t *dir_status);

Std_ReturnType gpio_port_write_logic(port_index_t port , uint8_t logic);
Std_ReturnType gpio_port_read_logic(port_index_t port , uint8_t *logic);
Std_ReturnType gpio_port_toggle_logic(port_index_t port);






/* Global Shared Variables */

#endif	/* HAL_GPIO_H */

