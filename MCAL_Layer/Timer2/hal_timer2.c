/* 
 * File:   hal_timer2.c
 * Author: ahmed
 *
 * Created on August 15, 2022, 1:09 PM
 */

#include "hal_timer2.h"


#if TIMER2_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler tmr2_InterruptHandler = NULL ;
#endif

static uint8_t preloaded_val = 0 ; 


Std_ReturnType hal_timer2_init(const timer2_config_t *_tmr2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        /* Disable Timer2 Module */
        TMR2_DISABLE();
        /* Set Pre-Scaler */
        TMR2_CONFIG_PRE_SELECT(_tmr2_config->pre_scaler_value);
        /* Set Post-Scaler */
        TMR2_CONFIG_POST_SELECT(_tmr2_config->post_scalar_value);

        /* Configure Interrupt */
        #if TIMER2_INT_ENABLE==FEATURE_ENABLE
        INT_TMR2_ENABLE();
        INT_TMR2_CLEAR_FLAG();
        tmr2_InterruptHandler = _tmr2_config->timer2_InterruptHandler ;
        
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_LOW_PRI == _tmr2_config->priority)
                {
                    INT_TMR2_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else if(INT_HIGH_PRI == _tmr2_config->priority)
                {
                    INT_TMR2_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();
                }
                else {}
        
            #else
                INT_PRI_FET_DIS();
                INT_GLOBAL_EN();
                INT_PERIPHERAL_EN();
            #endif
        #endif
        
        /* Load Preloaded Value */
        preloaded_val = _tmr2_config->pre_loaded_value ;
        TMR2 = preloaded_val ;
        PR2 = _tmr2_config->period_value ;
        
        /* Enable Module */
        TMR2_ENABLE();
    }
    
    return ret_value ;
}
Std_ReturnType hal_timer2_deinit(const timer2_config_t *_tmr2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        /* Disable Interrupt */
        #if TIMER2_INT_ENABLE==FEATURE_ENABLE
            INT_TMR2_DISABLE();
        #endif
        /* Disable Timer2 Module */
            TMR2_DISABLE();
    }
    
    return ret_value ;
}


Std_ReturnType hal_timer2_read_val(const timer2_config_t *_tmr2_config ,
        uint8_t *result)
{
    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == _tmr2_config) || (NULL == result))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        *result = TMR2 ;
    }
    
    return ret_value ;
}

Std_ReturnType hal_timer2_write_val(const timer2_config_t *_tmr2_config ,
        uint8_t val)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _tmr2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        TMR2 = val ;
    }
    
    return ret_value ;
}


void TMR2_ISR(void)
{
#if TIMER2_INT_ENABLE==FEATURE_ENABLE

    /* Clear Interrupt Flag */
    INT_TMR2_CLEAR_FLAG();
    
    /* Rewrite preloaded value */
    TMR2 = (uint8_t) (preloaded_val);
    PR2 = (uint8_t) (period_val);
    
    /* Execute Callback */
    if(tmr2_InterruptHandler)
    {
        tmr2_InterruptHandler();
    }
#endif
}
