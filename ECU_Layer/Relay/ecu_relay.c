/* 
 * File:   ecu_relay.c
 * Author: ahmed
 *
 * Created on July 11, 2022, 10:31 PM
 */
/* Includes */
#include "ecu_relay.h"

/* GLOBAL Variables */


/* Implementation */

/**
 * @brief initialize relay pin and write default logic on it
 * 
 * @param relay relay object being initialized  
 * @return Std_ReturnType
 *          (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */
Std_ReturnType ecu_relay_init(const relay_t *relay)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == relay)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        pin_config_t relay_pin = 
        {
            .port = relay->port,
            .pin = relay->pin,
            .direction = OUTPUT,
            .logic = relay->state
        };

        gpio_pin_init(&relay_pin);
    }
    return ret_value ;

}

/**
 * @brief turns on relay
 * 
 * @param relay relay object to turn on
 * @return Std_ReturnType 
 *          (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */

Std_ReturnType ecu_relay_turn_on(const relay_t *relay)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == relay)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        pin_config_t relay_pin = 
        {
            .port = relay->port,
            .pin = relay->pin,
            .direction = OUTPUT
        };
        gpio_pin_write_logic(&relay_pin , GPIO_HIGH);
        
    }
    return ret_value ;

}
/**
 * @brief turns off relay
 * 
 * @param relay relay object to turn off
 * @return Std_ReturnType 
 *          (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */
Std_ReturnType ecu_relay_turn_off(const relay_t *relay)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == relay)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
       pin_config_t relay_pin = 
        {
            .port = relay->port,
            .pin = relay->pin,
            .direction = OUTPUT
        };
        gpio_pin_write_logic(&relay_pin , GPIO_LOW);
        
    }
    return ret_value ;

}