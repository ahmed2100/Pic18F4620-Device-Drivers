/* 
 * File:   ecu_seven_segment.c
 * Author: ahmed
 *
 * Created on July 25, 2022, 1:41 PM
 */

/* Includes */
#include "ecu_seven_segment.h"

Std_ReturnType ecu_seven_segment_init(const segment_t *segment)
{
    Std_ReturnType ret_value = E_OK;
    uint8_t counter = 0 ;

    if(NULL == segment)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        for(counter = 0 ; counter < 4 ; counter++)
        {
            ret_value =  gpio_pin_init( &(segment->segment_pins[counter]) );
        }
    }

    return ret_value ;
}
Std_ReturnType ecu_seven_segment_write_number(const segment_t *segment , uint8_t number)
{
    Std_ReturnType ret_value = E_OK;
    uint8_t counter = 0 ;

    if((NULL == segment) || (number > 9) || (number < 0 ))
    {
        ret_value = E_NOT_OK ;
    }
     else
    {
        // for(counter = 0 ;counter < 4 ; counter++)
        // {
        //     if(number % 2 == 0)
        //     {
        //         gpio_pin_write_logic(&(segment->segment_pins[counter]) , LOW);
        //     }
        //     else if(number % 2 == 1)
        //     {
        //         gpio_pin_write_logic(&(segment->segment_pins[counter]) , HIGH);

        //     }
        //     number = (uint8_t) (number / 2);
        // }
        // or
        for(counter = 0 ;counter < 4 ; counter++)
        {
            gpio_pin_write_logic(&(segment->segment_pins[counter]) , (number>>counter) & 0x01);
        }
        
    }
    
    return ret_value ;
}