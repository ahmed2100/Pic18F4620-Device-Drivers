/* 
 * File:   ecu_led.c
 * Author: ahmed
 *
 * Created on July 11, 2022, 10:31 PM
 */

#include "ecu_led.h"








/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType
 *           (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */
Std_ReturnType ecu_led_init(const led_t *led)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == led)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // turn a led_t into pin_config_t
        pin_config_t led_pin_config = 
        {
            .pin = led->pin,
            .port = led->port_name,
            .direction = OUTPUT,
            .logic = led->led_status
        };
        gpio_pin_init(&led_pin_config);   // initialize the pin using gpio module  
    }
    return ret_value ;
}

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 *           (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */
Std_ReturnType ecu_led_turn_on(led_t *led)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == led)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // turn a led_t into pin_config_t
        pin_config_t led_pin_config = 
        {
            .pin = led->pin,
            .port = led->port_name,
            .direction = OUTPUT,
            .logic = led->led_status
        };
        led->led_status = LED_ON ;
        gpio_pin_write_logic(&led_pin_config , GPIO_HIGH);
    }

    return ret_value ;

}

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 *           (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */
Std_ReturnType ecu_led_turn_off(led_t *led)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == led)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // turn a led_t into pin_config_t
        pin_config_t led_pin_config = 
        {
            .pin = led->pin,
            .port = led->port_name,
            .direction = OUTPUT,
            .logic = led->led_status
        };
        led->led_status = LED_ON ;
        ret_value = gpio_pin_write_logic(&led_pin_config , GPIO_LOW);

    }

    return ret_value ;

}

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 *           (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation 
 */

Std_ReturnType ecu_led_turn_toggle(led_t *led)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == led)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {

        // turn a led_t into pin_config_t
        pin_config_t led_pin_config = 
        {
            .pin = led->pin,
            .port = led->port_name,
            .direction = OUTPUT,
            .logic = led->led_status
        };
        led->led_status = !(led->led_status) ;
        ret_value = gpio_pin_toggle_logic(&led_pin_config);
    }

    return ret_value ;

}







