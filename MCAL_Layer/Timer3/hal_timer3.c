/* 
 * File:   hal_timer3.c
 * Author: ahmed
 *
 * Created on August 16, 2022, 11:25 AM
 */
#include "hal_timer3.h"


#if TIMER3_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler tmr3_InterruptHandler = NULL ;
#endif

static uint16_t preloaded_val = 0 ; 



Std_ReturnType hal_timer3_init(const timer3_config_t *_tmr3_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr3_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Disable Timer3 Module */
        TMR3_DISABLE();
        
        /* Initialize preloaded value */
        preloaded_val = _tmr3_config->preloaded_value ;
        ret_value = hal_timer3_write_val(_tmr3_config , _tmr3_config->preloaded_value);
        
        /* Set Prescaler */
        TMR3_CONFIG_SELECT_PRESCALER(_tmr3_config->prescalar_value);
        
        /* Set Mode (Counter or Timer) */
        if(TMR3_COUNTER_MODE == _tmr3_config->timer_mode)
        {
            SET_BIT(TRISC , PIN0);
        }
        TMR3_CONFIG_SET_MODE(_tmr3_config->timer_mode);
        
        /* Set Synch */
        TMR3_CONFIG_SELECT_SYNCH(_tmr3_config->synchronization);
        
        /* Set read/write mode */
        TMR3_CONFIG_SELECT_RD_WR_MODE(_tmr3_config->read_write_mode);
        
        /* Configure Interrupt */
        #if TIMER3_INT_ENABLE==FEATURE_ENABLE
        INT_TMR3_CLEAR_FLAG();
        INT_TMR3_ENABLE();
        tmr3_InterruptHandler = _tmr3_config->timer3_InterruptHandler ;
        
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_LOW_PRI == _tmr3_config->priority)
                {
                    INT_TMR3_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else if(INT_HIGH_PRI == _tmr3_config->priority)
                {
                    INT_TMR3_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();
                }
                else {}
        
            #else
                INT_PRI_FET_DIS();
                INT_GLOBAL_EN();
                INT_PERIPHERAL_EN();
            #endif
        #endif
        
        
        
        /* Enable Timer3 Module and automatically start counting*/
        TMR3_ENABLE();
    }
    
    return ret_value ;
}


Std_ReturnType hal_timer3_deinit(const timer3_config_t *_tmr3_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr3_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Disable Interrupt */
        #if TIMER3_INT_ENABLE==FEATURE_ENABLE
            INT_TMR3_DISABLE();
        #endif
        /* Disable Module */
        TMR3_DISABLE();
    }
    
    return ret_value ;
}


Std_ReturnType hal_timer3_read_val(const timer3_config_t *_tmr3_config ,
        uint8_t *result)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_low_byte  = 0 ;
    uint8_t l_high_byte = 0 ;
    
    if((NULL == _tmr3_config) || (NULL == result) )
    {
        ret_value = E_NOT_OK ;  
    }
    else 
    {
        l_low_byte  = TMR3L ;
        l_high_byte = TMR3H ;
        *result = (uint16_t) ( l_low_byte + (l_high_byte << 8));
    }   
    
    return ret_value ;
    
}

Std_ReturnType hal_timer3_write_val(const timer3_config_t *_tmr3_config ,
        uint8_t val)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr3_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        TMR3H = (uint8_t) ( (val) >> 8);
        TMR3L = (uint8_t) (val);
    }
    
    return ret_value ;
    
}


void TMR3_ISR(void)
{
    /* Clear Interrupt Flag */
    INT_TMR3_CLEAR_FLAG();
    
    /* Rewrite preloaded value */
    TMR3H = (uint8_t) (preloaded_val >> 8);
    TMR3L = (uint8_t) (preloaded_val);
    
    /* Execute Callback */
    if(tmr3_InterruptHandler)
    {
        tmr3_InterruptHandler();
    }
}











