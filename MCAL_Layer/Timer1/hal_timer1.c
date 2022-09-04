/* 
 * File:   hal_timer1.h
 * Author: ahmed
 *
 * Created on August 14, 2022, 3:41 PM
 */

#include "hal_timer1.h"

#if TIMER1_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler tmr1_InterruptHandler = NULL ;
#endif

static uint16_t preloaded_val = 0 ; 


Std_ReturnType hal_timer1_init(const timer1_config_t *_tmr1_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr1_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Disable Timer1 Module */
        TMR1_DISABLE();
        
        /* Initialize preloaded value */
        preloaded_val = _tmr1_config->preloaded_value ;
        ret_value = hal_timer1_write_val(_tmr1_config , _tmr1_config->preloaded_value);
        
        /* Set Prescaler */
        TMR1_CONFIG_SELECT_PRESCALER(_tmr1_config->prescalar_value);
        
        /* Set Mode (Counter or Timer) */
        if(TMR1_COUNTER_MODE == _tmr1_config->timer_mode)
        {
            SET_BIT(TRISC , PIN0);
        }
        TMR1_CONFIG_SET_MODE(_tmr1_config->timer_mode);
        
        /* Set Synch */
        TMR1_CONFIG_SELECT_SYNCH(_tmr1_config->synchronization);
        
        /* Set read/write mode */
        TMR1_CONFIG_SELECT_RD_WR_MODE(_tmr1_config->read_write_mode);
        
        /* Configure timer1 OSC */
        TMR1_CONFIG_OSC(_tmr1_config->timer1_osc_config);

        
        /* Configure Interrupt */
        #if TIMER1_INT_ENABLE==FEATURE_ENABLE
        INT_TMR1_CLEAR_FLAG();
        INT_TMR1_ENABLE();
        tmr1_InterruptHandler = _tmr1_config->timer1_InterruptHandler ;
        
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_LOW_PRI == _tmr1_config->priority)
                {
                    INT_TMR1_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else if(INT_HIGH_PRI == _tmr1_config->priority)
                {
                    INT_TMR1_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();
                }
                else {}
        
            #else
                INT_PRI_FET_DIS();
                INT_GLOBAL_EN();
                INT_PERIPHERAL_EN();
            #endif
        #endif
        
        
        
        /* Enable Timer1 Module and automatically start counting*/
        TMR1_ENABLE();
    }
    
    return ret_value ;
}

Std_ReturnType hal_timer1_deinit(const timer1_config_t *_tmr1_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr1_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Disable Interrupt */
        #if TIMER1_INT_ENABLE==FEATURE_ENABLE
            INT_TMR1_DISABLE();
        #endif
        /* Disable Module */
        TMR1_DISABLE();
    }
    
    return ret_value ;
    
}


Std_ReturnType hal_timer1_read_val(const timer1_config_t *_tmr1_config ,
        uint16_t *result)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_low_byte  = 0 ;
    uint8_t l_high_byte = 0 ;
    
    if((NULL == _tmr1_config) || (NULL == result) )
    {
        ret_value = E_NOT_OK ;  
    }
    else 
    {
        l_low_byte  = TMR1L ;
        l_high_byte = TMR1H ;
        *result = (uint16_t) ( l_low_byte + (l_high_byte << 8));
    }   
    
    return ret_value ;
    
}

Std_ReturnType hal_timer1_write_val(const timer1_config_t *_tmr1_config ,
        uint16_t val)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr1_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        TMR1H = (uint8_t) ( (val) >> 8);
        TMR1L = (uint8_t) (val);
    }
    
    return ret_value ;
}

void TMR1_ISR(void)
{
    #if TIMER1_INT_ENABLE==FEATURE_ENABLE
    /* Clear Interrupt Flag */
    INT_TMR1_CLEAR_FLAG();
    
    /* Rewrite preloaded value */
    TMR1H = (uint8_t) (preloaded_val >> 8);
    TMR1L = (uint8_t) (preloaded_val);
    /* Execute Callback */
    if(tmr1_InterruptHandler)
    {
        tmr1_InterruptHandler();
    }
    #endif
}

