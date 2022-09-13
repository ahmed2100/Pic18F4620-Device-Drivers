/* 
 * File:   ecu_servo_motor.c
 * Author: ahmed
 *
 * Created on September 9, 2022, 4:13 PM
 */

#include "ecu_servo_motor.h"

static inline void PCA9685_write_reg(const  mssp_i2c_t *i2c_obj , i2c_slave_addr_t i2c_addr , uint8_t control_reg , uint8_t reg_byte);
static inline void PCA9685_read_reg(const  mssp_i2c_t *i2c_obj , i2c_slave_addr_t i2c_addr , uint8_t control_reg , uint8_t *reg_byte);

static inline void PCA9685_set_PWM_freq(const  mssp_i2c_t *i2c_obj , i2c_slave_addr_t i2c_addr , uint32_t frequency);


/**
 * 
 * @brief Interface to initialize the PCA9685 for servo motor control.
 * 
 * @param PCA9685_obj pointer to PCA9685 device object.
 * @param i2c_obj pointer to i2c object used for communication over i2c bus

 * @return Status of the function
 *          (E_OK) : The function done successfully.
 *          (E_NOT_OK) : The function had an issue performing the operation.
 */
Std_ReturnType PCA9685_servo_init(const PCA9685_servo_driver_t *PCA9685_obj ,const  mssp_i2c_t *i2c_obj)
{
    Std_ReturnType ret_val = E_OK ;
    
    if((NULL == PCA9685_obj) || (NULL == i2c_obj ))
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* configure mode 1 :
         * register AI = 1 ===> enable
         * SLEEP = 0
         *  
         */
        PCA9685_write_reg(i2c_obj , PCA9685_obj->i2c_addr , ECU_SM_MODE1_REG , ECU_SM_MODE1_REG_INIT_VAL );
        
        
        /* configure mode 2 :
         * OUTDRV = 1 ===> totem pole config
         * 
         *  */
        PCA9685_write_reg(i2c_obj , PCA9685_obj->i2c_addr , ECU_SM_MODE2_REG , ECU_SM_MODE2_REG_INIT_VAL );

        /* wait for OSC startup */
        __delay_us(500);
        /* Configure PWM freq. */
        PCA9685_set_PWM_freq(i2c_obj , PCA9685_obj->i2c_addr ,PCA9685_obj->PWM_freq );
        
    }
    
    return ret_val ;
}

/**
 * 
 * @brief Interface to write an angle from (0 - 360)degree to servo
 * 
 * @note For servo motors of 180 degree max. the application code must limit the angle value specified because 
 *       if you try to force a servo beyond its limits it will get very hot (possibly to destruction) 
 *       and may strip its gears.
 * 
 * 
 * @param PCA9685_obj pointer to PCA9685 device object.
 * @param servo_index Index of servo motor.
 * @param servo_angle Angle to write to servo motor.
 * @return Status of the function
 *          (E_OK) : The function done successfully.
 *          (E_NOT_OK) : The function had an issue performing the operation.
 */
Std_ReturnType PCA9685_servo_write_angle(const PCA9685_servo_driver_t *PCA9685_obj ,const mssp_i2c_t *i2c_obj , uint8_t servo_index , uint16_t servo_angle)
{
    Std_ReturnType ret_val = E_OK ;
    
    if((NULL == PCA9685_obj ) || (NULL == i2c_obj))
    {
        ret_val = E_NOT_OK ;
                
    }
    else
    {
        
    }
    
    return ret_val;
    
}

/**
 * 
 * @brief Interface to read angle on servo motor.
 * 
 * 
 * @param PCA9685_obj pointer to PCA9685 device object.
 * @param servo_index Index of servo motor.
 * @param servo_angle pointer to return the servo motor angle.
 * @return Status of the function
 *          (E_OK) : The function done successfully.
 *          (E_NOT_OK) : The function had an issue performing the operation.
 */
Std_ReturnType PCA9685_servo_read_angle(const PCA9685_servo_driver_t *PCA9685_obj ,const mssp_i2c_t *i2c_obj , uint8_t servo_index , uint16_t *servo_angle)
{
    Std_ReturnType ret_val = E_OK ;
    
    if((NULL == PCA9685_obj ) || (NULL == servo_angle) || (NULL == i2c_obj))
    {
        ret_val = E_NOT_OK ;
                
    }
    else
    {
        
    }
    
    return ret_val;
}

static inline void PCA9685_write_reg(const  mssp_i2c_t *i2c_obj , i2c_slave_addr_t i2c_addr , uint8_t control_reg , uint8_t reg_byte)
{
    uint8_t ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE ;
    
    /* Send Start */
    mssp_i2c_master_send_start_cond(i2c_obj);
    /* Send PCA9685 I2C bus address */
    mssp_i2c_master_write_data_blocking(i2c_obj , i2c_addr.full_byte , &ack );
    if(ack == I2C_ACK_RECEIVED_FROM_SLAVE)
    {
        /* Send control register to be written to */
        mssp_i2c_master_write_data_blocking(i2c_obj , control_reg , &ack );
    }
    if(ack == I2C_ACK_RECEIVED_FROM_SLAVE)
    {
        /* write byte to register */
        mssp_i2c_master_write_data_blocking(i2c_obj , reg_byte , &ack );
    }

    mssp_i2c_master_send_stop_cond(i2c_obj);    
    
}

static inline void PCA9685_read_reg(const  mssp_i2c_t *i2c_obj , i2c_slave_addr_t i2c_addr , uint8_t control_reg , uint8_t *reg_byte)
{
    uint8_t ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE ;
    
    i2c_addr.read_write_bit = 0 ; // write

    /* Send Start */
    mssp_i2c_master_send_start_cond(i2c_obj);
    /* Send PCA9685 I2C bus address */
    mssp_i2c_master_write_data_blocking(i2c_obj , i2c_addr.full_byte , &ack );
    if(ack == I2C_ACK_RECEIVED_FROM_SLAVE)
    {
        mssp_i2c_master_write_data_blocking(i2c_obj , ECU_SM_MODE1_REG , &ack );

    }
    if(ack == I2C_ACK_RECEIVED_FROM_SLAVE)
    {   
        i2c_addr.read_write_bit = 1 ; // read
        mssp_i2c_master_send_re_start_cond(i2c_obj);
        mssp_i2c_master_write_data_blocking(i2c_obj , i2c_addr.full_byte , &ack);
        
       
    }
    if(ack == I2C_ACK_RECEIVED_FROM_SLAVE)
    {
         /* read register byte to reg_byte */
        mssp_i2c_master_read_data_blocking(i2c_obj , reg_byte , I2C_MASTER_SEND_NACK );
    }
    
    
    
    
    if(ack == I2C_ACK_RECEIVED_FROM_SLAVE)
    {
    
    }

    mssp_i2c_master_send_stop_cond(i2c_obj);    
    
}

static inline void PCA9685_set_PWM_freq(const  mssp_i2c_t *i2c_obj , i2c_slave_addr_t i2c_addr , uint32_t frequency)
{
    uint8_t mode1 = 0x00 ;
    uint8_t pre_scaler_value = 0 ;
     
    pre_scaler_value = (uint8_t) ((ECU_SM_INT_OSC / (4096.0 * frequency)) -1) ;
    // preserve original mode
    PCA9685_read_reg(i2c_obj , i2c_addr , ECU_SM_MODE1_REG , &mode1);
    PCA9685_write_reg(i2c_obj , i2c_addr , ECU_SM_MODE1_REG ,(mode1 & ~ECU_SM_MODE1_RESTART) | ECU_SM_MODE1_SLEEP);
    PCA9685_write_reg(i2c_obj , i2c_addr , ECU_SM_PRE_SCALE_REG , pre_scaler_value);
    PCA9685_write_reg(i2c_obj , i2c_addr , ECU_SM_MODE1_REG , mode1);
    
}