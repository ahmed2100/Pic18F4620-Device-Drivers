/* 
 * File:   ecu_dc_motor.c
 * Author: ahmed
 *
 * Created on July 23, 2022, 8:35 AM
 */

#include "ecu_dc_motor.h"

/* Global Variables*/


/**
 * @brief 
 * 
 * @param dc_motor 
 * @return Std_ReturnType 
 */
Std_ReturnType ecu_dc_motor_init(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == dc_motor)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        gpio_pin_init(&(dc_motor->dc_motor[DC_MOTOR_LEFT_PIN]));
        gpio_pin_init(&(dc_motor->dc_motor[DC_MOTOR_RIGHT_PIN]));
    }

    return ret_value ;
}

/**
 * @brief 
 * @param dc_motor
 * @return Std_ReturnType
 */
Std_ReturnType ecu_dc_motor_run_left(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == dc_motor)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor[DC_MOTOR_LEFT_PIN]) , GPIO_HIGH );
        gpio_pin_write_logic(&(dc_motor->dc_motor[DC_MOTOR_RIGHT_PIN]) , GPIO_LOW );
    }

    return ret_value ;
}
/**
 * @brief 
 * 
 * @param dc_motor 
 * @return Std_ReturnType 
 */
Std_ReturnType ecu_dc_motor_run_right(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == dc_motor)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor[DC_MOTOR_LEFT_PIN]) , GPIO_LOW );
        gpio_pin_write_logic(&(dc_motor->dc_motor[DC_MOTOR_RIGHT_PIN]) , GPIO_HIGH );
    }

    return ret_value ;
}
/**
 * @brief 
 * 
 * @param dc_motor 
 * @return Std_ReturnType 
 */
Std_ReturnType ecu_dc_motor_stop(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == dc_motor)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor[DC_MOTOR_LEFT_PIN]) , GPIO_LOW );
        gpio_pin_write_logic(&(dc_motor->dc_motor[DC_MOTOR_RIGHT_PIN]) , GPIO_LOW );
    }

    return ret_value ;
}