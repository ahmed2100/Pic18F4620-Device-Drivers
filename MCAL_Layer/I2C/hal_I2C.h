/* 
 * File:   hal_I2C.h
 * Author: ahmed
 *
 * Created on September 2, 2022, 10:37 AM
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H


/******************************************** Includes ********************************************/

#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "hal_I2C_config.h"

/******************************************** Macro Defines ***************************************************************/

/* Selectable i2c modes */
#define I2C_MODE_SLAVE_7BIT                 0x06
#define I2C_MODE_SLAVE_10BIT                0x07
#define I2C_MODE_MASTER_DEFINED_CLOCK       0x08
#define I2C_MODE_MASTER_FIRMWARE_CON        0x0B
#define I2C_MODE_SLAVE_7BIT_WITH_SS_INTs    0x0E
#define I2C_MODE_SLAVE_10BIT_WITH_SS_INTs   0x0F

/* Master or slave modes */
#define I2C_MODE_MASTER                     0x01
#define I2C_MODE_SLAVE                      0x00

/* Disable and Enable slew rates */
#define I2C_SLEW_RATE_ENABLE                0x00
#define I2C_SLEW_RATE_DISABLE               0x01

/* Disable and Enable SMBus inputs */
#define I2C_SMBUS_INPUTS_ENABLE             0x01
#define I2C_SMBUS_INPUTS_DISABLE            0x00

/* Slave mode data or address */
#define I2C_LAST_BYTE_DATA                  0x01
#define I2C_LAST_BYTE_ADDR                  0x00

/* START cond detected or not */
#define I2C_START_COND_DETECTED             0x01
#define I2C_START_COND_NOT_DETECTED         0x00

/* STOP cond detected or not */
#define I2C_STOP_COND_DETECTED             0x01
#define I2C_STOP_COND_NOT_DETECTED         0x00

/* Enable and disable general call */
#define I2C_GENERAL_CALL_ENABLE            0x01
#define I2C_GENERAL_CALL_DISABLE           0x00

/* Master mode enable and disable receive mode */
#define I2C_RECEIVE_MODE_ENABLE            0x01
#define I2C_RECEIVE_MODE_DISABLE           0x00

/* Acknowledge Status bit (Master Transmit mode only) */
#define I2C_ACK_NOT_RECEIVED_FROM_SLAVE    0x01
#define I2C_ACK_RECEIVED_FROM_SLAVE        0x00

/* Receive Enable bit (Master mode only) */
#define I2C_MASTER_RECEIVER_ENABLE         0x01
#define I2C_MASTER_RECEIVER_DISABLE        0x00

/* Acknowledge Data bit (Master Receive mode only) */
#define I2C_MASTER_SEND_ACK                0x00
#define I2C_MASTER_SEND_NACK               0x01

/* Receive Overflow Indicator */
#define I2C_RECEIVE_MODE_OVERFLOW          0x01
#define I2C_RECEIVE_MODE_NO_OVERFLOW       0x00

/* Write Collision Indicator */
#define I2C_WRITE_COL_OCCURED              0x01
#define I2C_WRITE_COL_NOT_OCCURED          0x00

/* Buffer Full Status */
#define I2C_BUFFER_FULL                    0x01
#define I2C_BUFFER_EMPTY                   0x00

/******************************************** Macro Functions *************************************************************/

/* Enable and Disable MSSP module */
#define MSSP_ENABLE()                       (SSPCON1bits.SSPEN = 1)
#define MSSP_DISABLE()                      (SSPCON1bits.SSPEN = 0)

/* Clear Receive Overflow */
#define MSSP_I2C_CLEAR_REC_OV()             (SSPCON1bits.SSPOV = 0)
/* Clear Write Collision */
#define MSSP_I2C_CLEAR_WR_COL()             (SSPCON1bits.WCOL = 0)

/* Slave Mode */
#define MSSP_I2C_SLAVE_RELEASE_CLK()        (SSPCON1bits.CKP =  1)
#define MSSP_I2C_SLAVE_STRETCH_CLK()        (SSPCON1bits.CKP =  0)

/* Slave Mode enable clock stretching */
#define MSSP_I2C_SLAVE_CLK_STRETCHING_ENABLE() (SSPCON2bits.SEN = 1)




/******************************************** User-Defined Data Type Declaration ********************************************/

typedef union {
    struct {
        uint8_t read_write_bit : 1 ;
        uint8_t address : 7 ;
    };
    uint8_t full_byte ;
}i2c_slave_addr_ut;

typedef struct {
    i2c_slave_addr_ut i2c_slave_address ;
    uint8_t i2c_10bit_address_second_byte ;
    uint8_t i2c_mode_cfg ;
    uint8_t i2c_mode : 1 ;
    uint8_t i2c_slew_rate : 1 ;
    uint8_t i2c_general_call : 1 ;
    uint8_t i2c_SMBus_control : 1 ;
    uint8_t i2c_master_rec_mode : 1 ;
#if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE
#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
    uint8_t i2c_priority : 1 ;
#endif
#endif
    
#if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE
#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
    uint8_t i2c_bus_col_priority : 1 ;
#endif
#endif
    uint8_t i2c_reserved : 1 ;
    
    
}i2c_configs_st ;

typedef struct 
{
#if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE
    InterruptHandler i2c_InterruptHandler ;  
    InterruptHandler i2c_Receive_OverFlow_callback ;
    InterruptHandler i2c_Write_Collision_callback ;
#endif
#if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE
    InterruptHandler i2c_Bus_Colision_InterruptHandler ;
#endif       

    uint32_t i2c_clock_freq ;
    i2c_configs_st i2c_config ;


    

}mssp_i2c_st;




/******************************************** Software Interfaces (Prototypes) ********************************************/

Std_ReturnType MSSP_I2C_Deinit(const mssp_i2c_st *i2c_obj);
Std_ReturnType MSSP_I2C_Init(const mssp_i2c_st *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Start_Cond(const mssp_i2c_st *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Re_Start_Cond(const mssp_i2c_st *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Stop_Cond(const mssp_i2c_st *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Write_Data_Blocking(const mssp_i2c_st *i2c_obj , uint8_t i2c_data , uint8_t *i2c_ack);
Std_ReturnType MSSP_I2C_Master_Read_Data_Blocking(const mssp_i2c_st *i2c_obj , uint8_t *i2c_data , uint8_t ack);





/******************************************** Global Shared Variables ********************************************/



#endif	/* HAL_I2C_H */

