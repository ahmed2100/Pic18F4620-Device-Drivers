/* 
 * File:   hal_timer0.c
 * Author: ahmed
 *
 * Created on August 13, 2022, 7:19 AM
 */
#include "hal_timer0.h"

#if TIMER0_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler tmr0_InterruptHandler = NULL ;
#endif

static uint16_t preloaded_val = 0 ;    
    

/**
 * @brief this interface initializes timer0 module with the config
 *        specified in _tmr0_config
 * 
 * @param _tmr0_config : configuration to initialize module
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_timer0_init(const timer0_config_t *_tmr0_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr0_config)
    {
        ret_value = E_NOT_OK ; 
    }
    else 
    {
        /* Disable timer0 module */
        TMR0_DISABLE();
        /* configure 8bit or 16bit based on config in config header file   */
        TMR0_CONFIG_SET_REG_SZ(_tmr0_config->reg_bit_size);
        /* configure prescalar value */
        if(TMR0_PRESCALER_ON ==  _tmr0_config->prescaler_enable )
        {
            TMR0_CONFIG_PRESCALER_SET_VAL(_tmr0_config->prescalar_value);
            TMR0_CONFIG_PRESCALER_ENABLE();
        }
        else if(TMR0_PRESCALER_OFF == _tmr0_config->prescaler_enable)
        {
            TMR0_CONFIG_PRESCALER_DISABLE();
        }
        else {/* Nothing */}
        
        /* configure mode (counter or timer) */
        if( TMR0_COUNTER_MODE == _tmr0_config->timer_mode)
        {
            /* configure edge */
             TMR0_CONFIG_SET_EDGE(_tmr0_config->edge);
             
            /* configure T0CKL to be input  */
            SET_BIT(TRISA , PIN4);
        }
        TMR0_CONFIG_SET_MODE(_tmr0_config->timer_mode);
        
        /* Set Pre-loaded Value */
        ret_value = hal_timer0_write_val(_tmr0_config , _tmr0_config->preloaded_value);
        preloaded_val = _tmr0_config->preloaded_value ;
        
        /* configure interrupt and set priority */
        #if TIMER0_INT_ENABLE==FEATURE_ENABLE
        /* configure interrupt */
            INT_TMR0_ENABLE();
            INT_TMR0_CLEAR_FLAG();
            tmr0_InterruptHandler = _tmr0_config->timer0_InterruptHandler ;
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                /* Priority Config */
                INT_PRI_FET_EN();
                switch(_tmr0_config->priority)
                {
                    case INT_HIGH_PRI:
                        INT_TMR0_PRI_SET_HIGH();
                        INT_GLOBAL_HIGH_EN(); 
                        break;
                    case INT_LOW_PRI:
                        INT_TMR0_PRI_SET_LOW();
                        INT_GLOBAL_HIGH_EN(); 
                        INT_GLOBAL_LOW_EN(); 
                        break;
                }
                        
            #else
                INT_PRI_FET_DIS();
                INT_PERIPHERAL_EN();
                INT_GLOBAL_EN();
            #endif
        
        #endif

        /* Enable Timer0 and start timing or counting */
        TMR0_ENABLE();
    }
    
    return ret_value ;
}
/**
 * @brief disables timer0 module
 * 
 * @param _tmr0_config
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_timer0_deinit(const timer0_config_t *_tmr0_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr0_config)
    {
        ret_value = E_NOT_OK ; 
    }
    else 
    {
        /* disable interrupt */
        #if TIMER0_INT_ENABLE==FEATURE_ENABLE
            INT_TMR0_DISABLE();
        #endif
        /* disable module */
        TMR0_DISABLE();
    } 
    
    return ret_value ;
    
}

/**
 * 
 * @param _tmr0_config
 * @param result
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */

Std_ReturnType hal_timer0_read_val(const timer0_config_t *_tmr0_config , 
                                            uint16_t *result)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_low_byte = 0;
    uint8_t l_high_byte = 0;
    
    if((NULL == _tmr0_config) || (NULL == result))
    {
        ret_value = E_NOT_OK ; 
    }
    else 
    {
        if( TMR0_16BIT ==  _tmr0_config->reg_bit_size)
        {
            l_low_byte  = TMR0L ;
            l_high_byte = TMR0H ;
            *result     = (uint16_t) (l_low_byte + (l_high_byte << 8));
        }
        else if(TMR0_8BIT ==  _tmr0_config->reg_bit_size)
        {
            l_low_byte = TMR0L ;
            *result = (uint16_t) (l_low_byte);
        }
        else { ret_value = E_NOT_OK ; }
    }
    
    return ret_value ;
    
}
/**
 * 
 * @param _tmr0_config
 * @param count
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_timer0_write_val(const timer0_config_t *_tmr0_config ,
                                      uint16_t val)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr0_config)
    {
        ret_value = E_NOT_OK ; 
    }
    else 
    {
        
        if( TMR0_16BIT ==  _tmr0_config->reg_bit_size)
        {
            /* + 2 because of :
             *  If the TMR0 register is written to, the increment is inhibited
             *  for the following two instruction cycles (datasheet: 11.1)
             */
            TMR0H = (uint8_t) (( (val + 2) >> 8));
            TMR0L = (uint8_t) (  (val + 2) & 0x00ff);
        }
        else if(TMR0_8BIT ==  _tmr0_config->reg_bit_size)
        {
            /* + 2 because of :
             *  If the TMR0 register is written to, the increment is inhibited
             *  for the following two instruction cycles (datasheet: 11.1)
             */
            TMR0L = (uint8_t) ((val + 2) & 0x00ff);
        }
        else { ret_value = E_NOT_OK ; }
        
    }
    
    return ret_value ;
    
}


void TMR0_ISR(void)
{
    INT_TMR0_CLEAR_FLAG();
    
    /* reload preloaded value */
    /* + 2 because of :
     *  If the TMR0 register is written to, the increment is inhibited
     *  for the following two instruction cycles (datasheet: 11.1)
     */
    
    TMR0H = (uint8_t) (( (preloaded_val + 2) >> 8));
    TMR0L = (uint8_t) ((preloaded_val + 2) & 0x00ff);
    
    if(tmr0_InterruptHandler)
    {
        tmr0_InterruptHandler();
    }
}