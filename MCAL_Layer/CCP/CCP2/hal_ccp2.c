/* 
 * File:   hal_ccp2.c
 * Author: ahmed
 *
 * Created on August 18, 2022, 10:58 AM
 */
#include "hal_ccp2.h"

#if CCP2_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler ccp2_InterruptHandler = NULL ;
#endif

    


static inline Std_ReturnType ccp2_capture_mode_select_variant(uint8_t ccp2_capture_mode);
static inline Std_ReturnType ccp2_compare_mode_select_variant(uint8_t ccp2_capture_mode);
static inline Std_ReturnType ccp2_PWM_set_DC(const ccp2_config_t *_ccp2_config);





Std_ReturnType hal_ccp2_deinit(const ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Reset CCP2 Module */
        CCP2_RESET();
        /* Disable Interrupt */
        INT_CCP2_DISABLE();    
    }
    
    return ret_value ;
}


#if CCP2_MODE_SELECTED==CAPTURE_CONFIG_MODE
Std_ReturnType hal_ccp2_capture_mode_init(const ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Reset CCP2 Module */
        CCP2_RESET();
        /* Set Mode */
        ret_value = ccp2_capture_mode_select_variant(_ccp2_config->ccp2_capture_mode_variant);
        /* Select Timer */

        
        /* Configure Pin to be input */
        if(CCP2_RC1_PIN == _ccp2_config->ccp2_pin)
        {
            SET_BIT(TRISC , 1);        
        }
        else if(CCP2_RB3_PIN == _ccp2_config->ccp2_pin)
        {
            SET_BIT(TRISB , 3);        

        }
        else {}
        /* Set Callback */
        ccp2_InterruptHandler = _ccp2_config->ccp2_InterruptHandler ;
        /* Enable Interrupt */
            #if CCP2_INT_ENABLE==FEATURE_ENABLE
        INT_CCP2_ENABLE();
        INT_CCP2_CLEAR_FLAG();
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_PRI_HIGH == _ccp2_config->priority)
                {
                    INT_CCP2_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();

                }
                else if(INT_PRI_LOW == _ccp2_config->priority)
                {
                    INT_CCP2_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else { /* Nothing */ }
            #else
                INT_PRI_FET_DIS();
                INT_PERIPHERAL_EN();
                INT_GLOBAL_EN();
            #endif  
        #endif

        
    }
    
    return ret_value ;
}


Std_ReturnType hal_ccp2_capture_mode_read(const ccp2_config_t *_ccp2_config , uint16_t *result)
{
    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == _ccp2_config) || (NULL == result))
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        *result = CCPR2 ;
    }
    
    return ret_value ;
}

Std_ReturnType hal_ccp2_capture_mode_change_variant(const ccp2_config_t *_ccp2_config , uint8_t _variant)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        CCP2_RESET();
        /* Set Prescaler */
        ret_value = ccp2_capture_mode_select_variant(_variant);   
    }
    
    return ret_value ;
    
}
#endif

#if CCP2_MODE_SELECTED==COMPARE_CONFIG_MODE
Std_ReturnType hal_ccp2_compare_mode_init(const ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        
        /* Reset CCP2 Module */
        CCP2_RESET();
        /* Set Mode */
        ret_value = ccp2_compare_mode_select_variant(_ccp2_config->ccp2_compare_mode_variant);
        
        /* Configure Pin to be input */
        if(CCP2_RC1_PIN == _ccp2_config->ccp2_pin)
        {
            CLEAR_BIT(TRISC , 1);        
        }
        else if(CCP2_RB3_PIN == _ccp2_config->ccp2_pin)
        {
            CLEAR_BIT(TRISB , 3);        

        }      
        else {}

        /* Set Callback */
        ccp2_InterruptHandler = _ccp2_config->ccp2_InterruptHandler ;
        /* Enable Interrupt */
        #if CCP2_INT_ENABLE==FEATURE_ENABLE
        INT_CCP2_ENABLE();
        INT_CCP2_CLEAR_FLAG();
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_PRI_HIGH == _ccp2_config->priority)
                {
                    INT_CCP2_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();

                }
                else if(INT_PRI_LOW == _ccp2_config->priority)
                {
                    INT_CCP2_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else { /* Nothing */ }
            #else
                INT_PRI_FET_DIS();
                INT_PERIPHERAL_EN();
                INT_GLOBAL_EN();
            #endif  
        #endif
    }
    
    return ret_value ;
    
}

Std_ReturnType hal_ccp2_compare_mode_set_value(const ccp2_config_t *_ccp2_config , uint16_t val)
{
     Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        CCPR2 = val ;
    }
    
    return ret_value ;
    
}

Std_ReturnType hal_ccp2_compare_mode_change_variant(const ccp2_config_t *_ccp2_config , uint8_t _variant)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        CCP2_RESET();
        /* Set Variant */
        ret_value = ccp2_compare_mode_select_variant(_variant);   
    }
    
    return ret_value ;
    
}

#endif

#if CCP2_MODE_SELECTED==PWM_CONFIG_MODE
Std_ReturnType hal_ccp2_PWM_mode_init(ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
         
        /* Reset CCP2 Module */
        CCP2_RESET();
        /* Set in PWM mode */        
        CCP2_SET_MODE(CCP2_MODE_PWM);
        
        /* Set PR2 */
        _ccp2_config->timer2.period_value = (uint8_t) ((_XTAL_FREQ)/( (4) * (_ccp2_config->PWM_freq) * (_ccp2_config->pre_scaler) ) ) - 1 ;
        PR2 = _ccp2_config->timer2.period_value ;
        /* Configure Pin to be output */
        if(CCP2_RC1_PIN == _ccp2_config->ccp2_pin)
        {
            CLEAR_BIT(TRISC , 1);        
        }
        else if(CCP2_RB3_PIN == _ccp2_config->ccp2_pin)
        {
            CLEAR_BIT(TRISB , 3);        

        }
        else {}
        
        /* Set Duty cycle */
        ccp2_PWM_set_DC(_ccp2_config);
        /* Set Callback */
        ccp2_InterruptHandler = _ccp2_config->ccp2_InterruptHandler ;
        /* Enable Interrupt */
        #if CCP2_INT_ENABLE==FEATURE_ENABLE
        INT_CCP2_ENABLE();
        INT_CCP2_CLEAR_FLAG();
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_PRI_HIGH == _ccp2_config->priority)
                {
                    INT_CCP2_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();

                }
                else if(INT_PRI_LOW == _ccp2_config->priority)
                {
                    INT_CCP2_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else { /* Nothing */ }
            #else
                INT_PRI_FET_DIS();
                INT_PERIPHERAL_EN();
                INT_GLOBAL_EN();
            #endif  
        #endif
        
    }
    
    return ret_value ;
    
}
Std_ReturnType hal_ccp2_PWM_mode_change_DC(const ccp2_config_t *_ccp2_config , uint8_t _DC)
{
    Std_ReturnType ret_value = E_OK ;
    uint16_t l_temp_10bit_dc = 0 ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        l_temp_10bit_dc = ((PR2 + 1) * (_DC) * 4) / 100;
        
        CCP2CONbits.DC2B = (l_temp_10bit_dc & 0x0003);
        CCPR2L = (uint8_t) (l_temp_10bit_dc >> 2);
    }
    
    return ret_value ;
    
}



Std_ReturnType hal_ccp2_PWM_mode_start_PWM(const ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        ret_value = hal_timer2_init( &(_ccp2_config->timer2) );
    }
    
    return ret_value ;
    
}
Std_ReturnType hal_ccp2_PWM_mode_stop(const ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        ret_value = hal_timer2_deinit( &(_ccp2_config->timer2) );
    }
    
    return ret_value ;
}
#endif


void CCP2_ISR()
{
    INT_CCP2_CLEAR_FLAG();
    
    if(ccp2_InterruptHandler)
    {
        ccp2_InterruptHandler();  
    }
    
}

#if CCP2_MODE_SELECTED==CAPTURE_CONFIG_MODE
static inline Std_ReturnType ccp2_capture_mode_select_variant(uint8_t ccp2_capture_mode)
{
    
    Std_ReturnType ret_value = E_OK ;
    
        switch(ccp2_capture_mode)
        {
            case CCP_MODE_CAPTURE_EVERY_1_FALLING :
                CCP2_SET_MODE(CCP_MODE_CAPTURE_EVERY_1_FALLING);
                break ;
  
            case CCP_MODE_CAPTURE_EVERY_1_RISING :
                CCP2_SET_MODE(CCP_MODE_CAPTURE_EVERY_1_RISING);
                break ;
            
            case CCP_MODE_CAPTURE_EVERY_4_RISING :
                CCP2_SET_MODE(CCP_MODE_CAPTURE_EVERY_4_RISING);  
                break ;
                
            case CCP_MODE_CAPTURE_EVERY_16_RISING :
                CCP2_SET_MODE(CCP_MODE_CAPTURE_EVERY_16_RISING);                
                break ;
            default:
                ret_value = E_NOT_OK ;
            
            
        }  
    return ret_value ;
}
#endif

#if CCP2_MODE_SELECTED==COMPARE_CONFIG_MODE
static inline Std_ReturnType ccp2_compare_mode_select_variant(uint8_t ccp2_capture_mode)
{
    
    Std_ReturnType ret_value = E_OK ;
    
        switch(ccp2_capture_mode)
        {
            case CCP_MODE_COMPARE_TOGGLE_MATCH :
                CCP2_SET_MODE(CCP_MODE_COMPARE_TOGGLE_MATCH);
                break ;
  
            case CCP_MODE_COMPARE_TURN_HIGH :
                CCP2_SET_MODE(CCP_MODE_COMPARE_TURN_HIGH);
                break ;
            
            case CCP_MODE_COMPARE_TURN_LOW :
                CCP2_SET_MODE(CCP_MODE_COMPARE_TURN_LOW);  
                break ;
                
            case CCP_MODE_COMPARE_GEN_INT :
                CCP2_SET_MODE(CCP_MODE_COMPARE_GEN_INT);                
                break ;
                
            case CCP_MODE_COMPARE_GEN_EVENT :
                CCP2_SET_MODE(CCP_MODE_COMPARE_GEN_EVENT);                
                break ;         
            default:
                ret_value = E_NOT_OK ;
        }  
        
    return ret_value ;
}
#endif

#if CCP2_MODE_SELECTED==PWM_CONFIG_MODE

static inline Std_ReturnType ccp2_PWM_set_DC(const ccp2_config_t *_ccp2_config)
{
    Std_ReturnType ret_value = E_OK ;
    uint16_t l_temp_10bit_dc = 0 ;
    
    if(NULL == _ccp2_config)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        l_temp_10bit_dc = ((PR2 + 1) * (_ccp2_config->duty_cycle) * 4) / 100; 
        CCP2CONbits.DC2B = (l_temp_10bit_dc & 0x0003);
        CCPR2L = (uint8_t) (l_temp_10bit_dc >> 2);
    }   
    
    
    return ret_value ;
}
#endif




