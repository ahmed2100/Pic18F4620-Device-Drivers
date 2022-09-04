/* 
 * File:   ecu_keypad.c
 * Author: ahmed
 *
 * Created on July 26, 2022, 7:16 AM
 */


#include "ecu_keypad.h"

static const uint8_t keypad_values[NUMBER_OF_ROWS][NUMBER_OF_COLS] = {
                                                        {'7' , '8' , '9' , '/'},
                                                        {'4' , '5' , '6' , '*'},
                                                        {'1' , '2' , '3' , '-'},
                                                        {'#' , '0' , '=' , '+'}
};


Std_ReturnType ecu_keypad_init(const keypad_t *keypad)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t counter = 0 ;

    if(NULL == keypad)
    {
        ret_value = E_NOT_OK ;
    }

    else
    {
        // initialization
        for(counter = 0 ; counter < NUMBER_OF_ROWS ; counter++)
        {
            ret_value &= gpio_pin_init(&(keypad->row_pins[counter]));
        }

        for(counter = 0 ; counter < NUMBER_OF_COLS ; counter++)
        {
            ret_value &= gpio_pin_direction_init(&(keypad->columns_pins[counter]));
        }
    }
    return ret_value ;
}

Std_ReturnType ecu_keypad_get_value(const keypad_t *keypad , uint8_t *value)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t row_counter = 0 ;
    uint8_t col_counter = 0 ;
    logic_t logic = GPIO_LOW ;

    if((NULL == keypad) || (NULL == value))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
//        *value = -1 ; // no key pressed
        // write initial zero volt on all rows
        for(row_counter = 0 ; row_counter < NUMBER_OF_ROWS ; row_counter++)
        {
            gpio_pin_write_logic(&(keypad->row_pins[row_counter]) , GPIO_LOW);
        }
        
        for(row_counter = 0 ; row_counter < NUMBER_OF_ROWS ; row_counter++)
        {
            // activate row
            gpio_pin_write_logic(&(keypad->row_pins[row_counter]) , GPIO_HIGH);
            // check all columns
            for(col_counter = 0 ; col_counter < NUMBER_OF_COLS ; col_counter++)
            {
                gpio_pin_read_logic(&(keypad->columns_pins[col_counter]) , &logic);
                if(GPIO_HIGH == logic )
                {
                    *value = keypad_values[row_counter][col_counter];
                }
            }
            // deactivate row
            gpio_pin_write_logic(&(keypad->row_pins[row_counter]) , GPIO_LOW);
        }

    }
    return ret_value ;
}