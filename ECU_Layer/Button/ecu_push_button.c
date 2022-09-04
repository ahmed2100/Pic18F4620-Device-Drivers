/* 
 * File:   ecu_push_button.c
 * Author: ahmed
 *
 * Created on July 11, 2022, 10:31 PM
 */

/* Includes */
#include "ecu_push_button.h"


/* Implementation */

/**
 * 
 * @param btn
 * @return Status of function
 *          (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation
 */ 
Std_ReturnType ecu_btn_init(const btn_t *btn)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == btn)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        pin_config_t btn_pin_config = 
        {
            .pin = btn->pin,
            .port = btn->port,
            .logic = !(btn->connection),
            .direction = INPUT
        };

        ret_value = gpio_pin_direction_init(&btn_pin_config);

    }
    return ret_value;
}

/**
 * 
 * @param btn
 * @param btn_state
 * @return Status of function
 *          (E_OK) : The function done successfully
 *          (E_OK) : The function had an issue performing the operation
 */
Std_ReturnType ecu_btn_read_logic(const btn_t *btn, btn_state_t *btn_state) 
{
    Std_ReturnType ret_value = E_OK ;
    logic_t logic_on_pin = GPIO_LOW ;   

    if( (NULL == btn) || NULL == (btn_state) )
    {
        ret_value = E_NOT_OK ;
    }

    else
    {

        pin_config_t btn_pin_config = 
            {
                .pin = btn->pin,
                .port = btn->port,
                .direction = INPUT
            };
        gpio_pin_read_logic(&btn_pin_config , &logic_on_pin);

        if(BUTTON_ACTIVE_HIGH == btn->connection)
        {
            if(GPIO_HIGH == logic_on_pin)
            {
                *btn_state = BUTTON_PRESSED;
            }
            else
            {
                *btn_state = BUTTON_RELEASED;
            }
            
        }
        else if(BUTTON_ACTIVE_LOW == btn->connection)
        {

            if(GPIO_HIGH == logic_on_pin)
            {
                *btn_state = BUTTON_RELEASED;
            }
            else
            {
                *btn_state = BUTTON_PRESSED;
            }

        }
        else
        {
            ret_value = E_NOT_OK ;
        }
        
    }
    
    return ret_value;
}