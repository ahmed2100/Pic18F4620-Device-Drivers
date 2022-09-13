/* 
 * File:   ecu_servo_motor.h
 * Author: ahmed
 *
 * Created on September 9, 2022, 4:13 PM
 * 
 * This module driver is created for PCA9685.
 * 
 */

#ifndef ECU_SERVO_MOTOR_H
#define	ECU_SERVO_MOTOR_H


/******************************************** Includes ********************************************/
#include "../../MCAL_Layer/I2C/hal_I2C.h"

/******************************************** Macro Defines ***************************************************************/

#define ECU_SM_MODE1_REG                0x00
#define ECU_SM_MODE2_REG                0x01

#define ECU_SM_SUBADR1_REG              0x02
#define ECU_SM_SUBADR2_REG              0x03
#define ECU_SM_SUBADR3_REG              0x04

#define ECU_SM_ALLCALLADDR_REG          0x05

#define ECU_SM_LED0_ON_L_REG            0x06
#define ECU_SM_LED0_ON_H_REG            0x07
#define ECU_SM_LED0_OFF_L_REG           0x08
#define ECU_SM_LED0_OFF_H_REG           0x09

#define ECU_SM_LED1_ON_L_REG            0x0A
#define ECU_SM_LED1_ON_H_REG            0x0B
#define ECU_SM_LED1_OFF_L_REG           0x0C
#define ECU_SM_LED1_OFF_H_REG           0x0D

#define ECU_SM_LED2_ON_L_REG            0x0E
#define ECU_SM_LED2_ON_H_REG            0x0F
#define ECU_SM_LED2_OFF_L_REG           0x10
#define ECU_SM_LED2_OFF_H_REG           0x11

#define ECU_SM_LED3_ON_L_REG            0x12
#define ECU_SM_LED3_ON_H_REG            0x13
#define ECU_SM_LED3_OFF_L_REG           0x14
#define ECU_SM_LED3_OFF_H_REG           0x15

#define ECU_SM_LED4_ON_L_REG            0x16
#define ECU_SM_LED4_ON_H_REG            0x17
#define ECU_SM_LED4_OFF_L_REG           0x18
#define ECU_SM_LED4_OFF_H_REG           0x19

#define ECU_SM_LED5_ON_L_REG            0x1A
#define ECU_SM_LED5_ON_H_REG            0x1B
#define ECU_SM_LED5_OFF_L_REG           0x1C
#define ECU_SM_LED5_OFF_H_REG           0x1D

#define ECU_SM_LED6_ON_L_REG            0x1E
#define ECU_SM_LED6_ON_H_REG            0x1F
#define ECU_SM_LED6_OFF_L_REG           0x20
#define ECU_SM_LED6_OFF_H_REG           0x21

#define ECU_SM_LED7_ON_L_REG            0x22
#define ECU_SM_LED7_ON_H_REG            0x23
#define ECU_SM_LED7_OFF_L_REG           0x24
#define ECU_SM_LED7_OFF_H_REG           0x25

#define ECU_SM_LED8_ON_L_REG            0x26
#define ECU_SM_LED8_ON_H_REG            0x27
#define ECU_SM_LED8_OFF_L_REG           0x28
#define ECU_SM_LED8_OFF_H_REG           0x29

#define ECU_SM_LED9_ON_L_REG            0x2A
#define ECU_SM_LED9_ON_H_REG            0x2B
#define ECU_SM_LED9_OFF_L_REG           0x2C
#define ECU_SM_LED9_OFF_H_REG           0x2D

#define ECU_SM_LED10_ON_L_REG           0x2E
#define ECU_SM_LED10_ON_H_REG           0x2F
#define ECU_SM_LED10_OFF_L_REG          0x30
#define ECU_SM_LED10_OFF_H_REG          0x31

#define ECU_SM_LED11_ON_L_REG           0x32
#define ECU_SM_LED11_ON_H_REG           0x33
#define ECU_SM_LED11_OFF_L_REG          0x34
#define ECU_SM_LED11_OFF_H_REG          0x35

#define ECU_SM_LED12_ON_L_REG           0x36
#define ECU_SM_LED12_ON_H_REG           0x37
#define ECU_SM_LED12_OFF_L_REG          0x38
#define ECU_SM_LED12_OFF_H_REG          0x39

#define ECU_SM_LED13_ON_L_REG           0x3A
#define ECU_SM_LED13_ON_H_REG           0x3B
#define ECU_SM_LED13_OFF_L_REG          0x3C
#define ECU_SM_LED13_OFF_H_REG          0x3D

#define ECU_SM_LED14_ON_L_REG           0x3E
#define ECU_SM_LED14_ON_H_REG           0x3F
#define ECU_SM_LED14_OFF_L_REG          0x40
#define ECU_SM_LED14_OFF_H_REG          0x41

#define ECU_SM_LED15_ON_L_REG           0x42
#define ECU_SM_LED15_ON_H_REG           0x43
#define ECU_SM_LED15_OFF_L_REG          0x44
#define ECU_SM_LED15_OFF_H_REG          0x45

#define ECU_SM_ALL_LED_ON_L_REG         0xFA
#define ECU_SM_ALL_LED_ON_H_REG         0xFB
#define ECU_SM_ALL_LED_OFF_L_REG        0xFC
#define ECU_SM_ALL_LED_OFF_H_REG        0xFD

#define ECU_SM_PRE_SCALE_REG            0xFE


#define ECU_SM_MODE1_REG_INIT_VAL       0x21

#define ECU_SM_MODE2_REG_INIT_VAL       0x04


#define ECU_SM_MODE1_SLEEP              0b00010000
#define ECU_SM_MODE1_RESTART            0b10000000

#define ECU_SM_INT_OSC                  25000000UL

/******************************************** Macro Functions *************************************************************/


/******************************************** User-Defined Data Type Declaration ********************************************/

typedef enum {
    PCA9685_SERVO_INDEX_0,
    PCA9685_SERVO_INDEX_1,
    PCA9685_SERVO_INDEX_2,
    PCA9685_SERVO_INDEX_3,
    PCA9685_SERVO_INDEX_4,
    PCA9685_SERVO_INDEX_5,
    PCA9685_SERVO_INDEX_6,
    PCA9685_SERVO_INDEX_7,
    PCA9685_SERVO_INDEX_8,
    PCA9685_SERVO_INDEX_9,
    PCA9685_SERVO_INDEX_10,
    PCA9685_SERVO_INDEX_11,
    PCA9685_SERVO_INDEX_12,
    PCA9685_SERVO_INDEX_13,
    PCA9685_SERVO_INDEX_14,
    PCA9685_SERVO_INDEX_15            
}PCA9685_servo_index_t ;

typedef struct {
    uint32_t PWM_freq ;
    i2c_slave_addr_t i2c_addr ;
    
}PCA9685_servo_driver_t ;

/******************************************** Software Interfaces (Prototypes) ********************************************/

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
Std_ReturnType PCA9685_servo_init(const PCA9685_servo_driver_t *PCA9685_obj ,const  mssp_i2c_t *i2c_obj);
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
Std_ReturnType PCA9685_servo_write_angle(const PCA9685_servo_driver_t *PCA9685_obj ,const mssp_i2c_t *i2c_obj , uint8_t servo_index , uint16_t servo_angle);

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
Std_ReturnType PCA9685_servo_read_angle(const PCA9685_servo_driver_t *PCA9685_obj ,const mssp_i2c_t *i2c_obj , uint8_t servo_index , uint16_t *servo_angle);

/******************************************** Global Shared Variables ********************************************/

#endif	/* ECU_SERVO_MOTOR_H */

