/* 
 * File:   hal_I2C.h
 * Author: ahmed
 *
 * Created on September 2, 2022, 10:37 AM
 */

#include "hal_I2C.h"

#if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE

static InterruptHandler i2c_InterruptHandler = NULL ;
static InterruptHandler i2c_Receive_OverFlow_callback = NULL ;
static InterruptHandler i2c_Write_Collision_callback =  NULL ;

#endif

#if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE

static InterruptHandler i2c_bus_colision_InterruptHandler = NULL ;

#endif




static inline void i2c_master_mode_init(const mssp_i2c_t *i2c_obj);
static inline void i2c_slave_mode_init(const mssp_i2c_t *i2c_obj);
static inline void i2c_mode_gpio_config(void);

static inline void i2c_interrupt_config(const mssp_i2c_t *i2c_obj);
static inline void i2c_bus_colision_interrupt_config(const mssp_i2c_t *i2c_obj);




Std_ReturnType mssp_i2c_init(const mssp_i2c_t *i2c_obj)
{
     Std_ReturnType ret_val = E_OK ;
    
    if(NULL == i2c_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Disable Module */
        MSSP_DISABLE();
        if(I2C_MODE_MASTER == i2c_obj->i2c_config.i2c_mode)
        {
            /* MSSP I2C Master Mode Config */
            /* MSSP I2C Master Mode Clock Config */
            i2c_master_mode_init(i2c_obj);
        }
        else if(I2C_MODE_SLAVE == i2c_obj->i2c_config.i2c_mode)
        {
            /* MSSP I2C Slave Mode Config */
            /* MSSP I2C Slave Mode Address Config */
            /* MSSP I2C Slave Mode General Call Config */
            i2c_slave_mode_init(i2c_obj);
        }
        else {}
        /* MSSP I2C GPIO Config */
        i2c_mode_gpio_config();
        
        /* MSSP I2C Slew Rate Config */
        if(I2C_SLEW_RATE_ENABLE == i2c_obj->i2c_config.i2c_slew_rate)
        {
            SSPSTATbits.SMP = I2C_SLEW_RATE_ENABLE ;
        }
        else if (I2C_SLEW_RATE_DISABLE == i2c_obj->i2c_config.i2c_slew_rate)
        {
            SSPSTATbits.SMP = I2C_SLEW_RATE_DISABLE ;
        }
        else {}
        /* MSSP I2C SMBus Config */
        if(I2C_SMBUS_INPUTS_ENABLE == i2c_obj->i2c_config.i2c_SMBus_control)
        {
            SSPSTATbits.CKE = I2C_SMBUS_INPUTS_ENABLE ;
        }
        else if (I2C_SMBUS_INPUTS_DISABLE == i2c_obj->i2c_config.i2c_SMBus_control)
        {
            SSPSTATbits.CKE = I2C_SMBUS_INPUTS_DISABLE ;
        }
        else {}
        
        /* MSSP I2C Interrupts Config */
            i2c_interrupt_config(i2c_obj);

        /* MSSP I2C Bus Colision Interrupts Config */
        i2c_bus_colision_interrupt_config(i2c_obj);

        /* Enable Module */
        MSSP_ENABLE();
    }
    
    return ret_val ;
    
}
Std_ReturnType mssp_i2c_deinit(const mssp_i2c_t *i2c_obj)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == i2c_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        
        /* Disable Module */
        MSSP_DISABLE();
        /* Disable Interrupts*/
        #if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE
            INT_MSSP_I2C_DISABLE();
        #endif

        #if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE
            INT_MSSP_I2C_BUS_COL_DISABLE();
        #endif
    }
    
    return ret_val ;
    
}
Std_ReturnType mssp_i2c_master_send_start_cond(const mssp_i2c_t *i2c_obj)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == i2c_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Initiate Start condition */
        SSPCON2bits.SEN = 1 ;
        /* wait for start condition completion */
        while(SSPCON2bits.SEN);
        /* Clear SSPIF */
        PIR1bits.SSPIF = INT_CLEARED ;
        /* Check if Start condition is sent successfully */
        if(I2C_START_COND_DETECTED == SSPSTATbits.S)
        {
            ret_val = E_OK ;
        }
        else 
        {
            ret_val = E_NOT_OK ;
        }
    }
    
    return ret_val ;
    
}
Std_ReturnType mssp_i2c_master_send_re_start_cond(const mssp_i2c_t *i2c_obj)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == i2c_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Initiate STOP condition */
        SSPCON2bits.RSEN = 1 ;
        /* wait for STOP condition completion */
        while(SSPCON2bits.RSEN);
        /* Clear SSPIF */
        PIR1bits.SSPIF = INT_CLEARED ;
        
    }
    
    return ret_val ;
}
Std_ReturnType mssp_i2c_master_send_stop_cond(const mssp_i2c_t *i2c_obj)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == i2c_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Initiate STOP condition */
        SSPCON2bits.PEN = 1 ;
        /* wait for STOP condition completion */
        while(SSPCON2bits.PEN);
        /* Clear SSPIF */
        PIR1bits.SSPIF = INT_CLEARED ;
        /* Check if STOP condition is sent successfully */
        if(I2C_STOP_COND_DETECTED == SSPSTATbits.P)
        {
            ret_val = E_OK ;
        }
        else 
        {
            ret_val = E_NOT_OK ;
        }
        
    }
    
    return ret_val ;
}
Std_ReturnType mssp_i2c_master_write_data_blocking(const mssp_i2c_t *i2c_obj , uint8_t i2c_data , uint8_t *i2c_ack)
{
    Std_ReturnType ret_val = E_OK ;
    
    if((NULL == i2c_obj) || (NULL == i2c_ack))
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Write Data to buffer register */
        SSPBUF = i2c_data ;
        /* Wait for data transfer to be completed */
        while(SSPSTATbits.BF);
        /* Clear SSPIF */
        PIR1bits.SSPIF = INT_CLEARED ;
        /* Report Acknowledge  */
        if(I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
        {
            *i2c_ack = I2C_ACK_RECEIVED_FROM_SLAVE ;
        }
        else 
        {
            *i2c_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE ;
        }
    }
    
    return ret_val ;
}
Std_ReturnType mssp_i2c_master_read_data_blocking(const mssp_i2c_t *i2c_obj , uint8_t *i2c_data , uint8_t ack)
{
    Std_ReturnType ret_val = E_OK ;
    
    if((NULL == i2c_obj) || (NULL == i2c_data))
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Enable Master Receive mode */
        SSPCON2bits.RCEN = I2C_MASTER_RECEIVER_ENABLE ;
        /* Wait for Byte reception */
        while(!SSPSTATbits.BF);
        /* return received data */
        *i2c_data = SSPBUF ;
        /* Clear SSPIF */
        PIR1bits.SSPIF = INT_CLEARED ;
        /* Send acknowledge */
        if(I2C_MASTER_SEND_ACK == ack)
        {
            SSPCON2bits.ACKDT = I2C_MASTER_SEND_ACK ; /* Acknowledge */
            SSPCON2bits.ACKEN = 1 ; /* Initiate acknowledge  */
        }
        else if(I2C_MASTER_SEND_ACK == ack)
        {
            SSPCON2bits.ACKDT = I2C_MASTER_SEND_NACK ; /* Not acknowledge */
            SSPCON2bits.ACKEN = 1 ; /* Initiate acknowledge  */
        }
        else { /* Nothing */ }
    }
    
    return ret_val ;
}


static inline void i2c_master_mode_init(const mssp_i2c_t *i2c_obj)
{
    SSPCON1bits.SSPM = I2C_MODE_MASTER_DEFINED_CLOCK ;
    /* BGR configuration */
    SSPADD = (uint8_t) (((_XTAL_FREQ / 4.0) / i2c_obj->i2c_clock_freq) - 1 );
}

static inline void i2c_slave_mode_init(const mssp_i2c_t *i2c_obj)
{
    /* Clear overflow indicator */
    MSSP_I2C_CLEAR_REC_OV();
    /* Clear write collision indicator */
    MSSP_I2C_CLEAR_WR_COL();
    /* MSSP I2C Slave Mode Config */
    /* Release clock */
    MSSP_I2C_SLAVE_RELEASE_CLK();
    /* MSSP I2C Slave Mode Address Config */
    switch(i2c_obj->i2c_config.i2c_mode_cfg)
    {
        case I2C_MODE_SLAVE_7BIT:
        case I2C_MODE_SLAVE_7BIT_WITH_SS_INTs:
            
            SSPADD = (uint8_t) (i2c_obj->i2c_config.i2c_slave_address.full_byte); 
            break ;
        case I2C_MODE_SLAVE_10BIT:
        case I2C_MODE_SLAVE_10BIT_WITH_SS_INTs:
            /* 10bit address configuration */
            break ;
        default:
            break ;  
    }
    SSPCON1bits.SSPM = i2c_obj->i2c_config.i2c_mode_cfg ;
    
    /* enable clock stretching */
//    MSSP_I2C_SLAVE_CLK_STRETCHING_ENABLE();
    
    /* MSSP I2C Slave Mode General Call Config */
    if(I2C_GENERAL_CALL_ENABLE == i2c_obj->i2c_config.i2c_general_call)
    {
        SSPCON2bits.GCEN = I2C_GENERAL_CALL_ENABLE ;
    }
    else if (I2C_GENERAL_CALL_DISABLE == i2c_obj->i2c_config.i2c_general_call)
    {
        SSPCON2bits.GCEN = I2C_GENERAL_CALL_DISABLE ;
    }
    else {}
}

static inline void i2c_mode_gpio_config(void)
{
    TRISCbits.TRISC3 = INPUT ;
    TRISCbits.TRISC4 = INPUT ;
}


static inline void i2c_interrupt_config(const mssp_i2c_t *i2c_obj)
{
#if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE
    INT_MSSP_I2C_ENABLE();
    INT_MSSP_I2C_CLEAR_FLAG();
    
    i2c_InterruptHandler = i2c_obj->i2c_InterruptHandler ;
    i2c_Receive_OverFlow_callback = i2c_obj->i2c_Receive_OverFlow_callback ;
    i2c_Write_Collision_callback = i2c_obj->i2c_Write_Collision_callback ;
    #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        INT_PRI_FET_EN();
        if(INT_LOW_PRI == i2c_obj->i2c_config.i2c_priority)
                {
                    INT_MSSP_I2C_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else if(INT_HIGH_PRI == i2c_obj->i2c_config.i2c_priority)
                {
                    INT_MSSP_I2C_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();
                }
                else {}
        
            #else
                INT_PRI_FET_DIS();
                INT_GLOBAL_EN();
                INT_PERIPHERAL_EN();
            #endif
    
#endif  
}
static inline void i2c_bus_colision_interrupt_config(const mssp_i2c_t *i2c_obj)
{
#if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE
    INT_MSSP_I2C_BUS_COL_ENABLE();
    INT_MSSP_I2C_BUS_COL_CLEAR_FLAG();
    
    i2c_bus_colision_InterruptHandler = i2c_obj->i2c_Bus_Colision_InterruptHandler ;
    #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        INT_PRI_FET_EN();
        if(INT_LOW_PRI == i2c_obj->i2c_config.i2c_bus_col_priority)
                {
                    INT_MSSP_I2C_BUS_COL_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else if(INT_HIGH_PRI == i2c_obj->i2c_config.i2c_bus_col_priority)
                {
                    INT_MSSP_I2C_BUS_COL_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();
                }
                else {}
        
            #else
                INT_PRI_FET_DIS();
                INT_GLOBAL_EN();
                INT_PERIPHERAL_EN();
            #endif
    
    
#endif    
}

/* Called when : START condition / STOP condition / Transmission Complete / Reception Complete / Repeated START  */
void MSSP_I2C_ISR(void)
{
#if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE

    INT_MSSP_I2C_CLEAR_FLAG();
    if(i2c_InterruptHandler)
    {
        i2c_InterruptHandler();
    }
    
    if(I2C_RECEIVE_MODE_OVERFLOW == SSPCON1bits.SSPOV)
    {
        /* Clear Overflow */
        MSSP_I2C_CLEAR_REC_OV();
        if(i2c_Receive_OverFlow_callback)
        {
            i2c_Receive_OverFlow_callback();
        }
    }
    
    if(I2C_WRITE_COL_OCCURED == SSPCON1bits.WCOL)
    {
        /* Clear write collision */
        MSSP_I2C_CLEAR_WR_COL();
        if(i2c_Write_Collision_callback)
        {
            i2c_Write_Collision_callback();
        }
    }
#endif
}

void MSSP_I2C_BC_ISR(void)
{
    #if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE
        INT_MSSP_I2C_BUS_COL_CLEAR_FLAG();

        /* Some code */

        if(i2c_InterruptHandler)
        {
            i2c_bus_colision_InterruptHandler();
        }
    #endif
}

