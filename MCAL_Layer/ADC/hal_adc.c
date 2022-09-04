/* 
 * File:   hal_adc.c
 * Author: ahmed
 *
 * Created on August 9, 2022, 1:06 PM
 */

#include "hal_adc.h"


#if ADC_INT_ENABLE==FEATURE_ENABLE
InterruptHandler adc_InterruptHandler = NULL ;
//interrupt_pri_t adc_priority = INT_LOW_PRI ;
#endif

static inline Std_ReturnType adc_input_channel_port_configuration(adc_channel_t adc_channel);
static inline Std_ReturnType select_ref_voltage(const adc_config_t *_adc);
static inline Std_ReturnType select_result_format(const adc_config_t *_adc);

/**
 * @summery this interface must be called before any ADC routine
 * @preconditions None
 * @param _adc is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_adc_init(const adc_config_t *_adc)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == _adc)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* Disable ADC module */
        ADC_DISABLE();
        
        /* Set Acquisition Time */
        ADC_SET_ACQ_TIME(_adc->aqcuisition_time);
                
        /* Set Default Channel */
        ADC_SELECT_CHANNEL(_adc->adc_channel);
        ret_value = adc_input_channel_port_configuration(_adc->adc_channel);

        /* Set Clock  */
        ADC_SET_CLOCK(_adc->conversion_clock);
        
        /* Set Result Format */
        
        ret_value &= select_result_format(_adc);
        /* Set Voltage Ref */
        ret_value &= select_ref_voltage(_adc);
        
        
        /* Set Callback  */
        #if ADC_INT_ENABLE==FEATURE_ENABLE
            /* disable ADC interrupt  */
            INT_ADC_ENABLE();
            /* clear ADC flag */
            INT_ADC_CLEAR_FLAG();
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
                INT_PRI_FET_EN();
                if(INT_LOW_PRI == _adc->priority)
                {
                    /* Set priority */
                    INT_ADC_PRI_SET_LOW();
                    /* Enable */
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else if(INT_HIGH_PRI == _adc->priority)
                {
                    /* Set priority */ 
                    INT_ADC_PRI_SET_HIGH();
                    /* Enable */
                    INT_GLOBAL_HIGH_EN();
                }
            #else
                INT_PRI_FET_DIS();
                INT_GLOBAL_EN();
                INT_PERIPHERAL_EN();
            #endif
            adc_InterruptHandler = _adc->ADC_InterruptHandler ;
        #endif
        /* Enable ADC module */
        ret_value &= ADC_ENABLE();

    }
    
    return ret_value ;
}


/**
 * @brief disables ADC module
 * @param _adc is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_adc_de_init(const adc_config_t *_adc)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == _adc)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        /* disable ADC interrupt */
        #if ADC_INT_ENABLE==FEATURE_ENABLE
            INT_ADC_DISABLE();
        #endif
        /* disable ADC module */
        ADC_DISABLE();
        
    }
    
    return ret_value ; 
}

/**
 * @brief selects input channel to use as analog input for the ADC
 * @preconditions hal_adc_init
 * 
 * @param channel input channel to select
 * @param _adc is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType hal_adc_select_ch(const adc_config_t *_adc , adc_channel_t channel)
{
    Std_ReturnType ret_value = E_OK ;
        /* select new channel */
    if(NULL == _adc)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ADC_SELECT_CHANNEL(channel);
        ret_value = adc_input_channel_port_configuration(channel);
    }
    
    return ret_value ;  
}
/**
 * @brief starts ADC conversion
 * @preconditions hal_adc_init
 * 
 * @param _adc is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */

Std_ReturnType hal_adc_start_conv(const adc_config_t *_adc)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == _adc)
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        ADC_CONV_START();
    }
    return ret_value ;
    
}

/**
 * @brief gets conversion status
 * @preconditions hal_adc_init  , hal_adc_start_conv
 *
 * @param conv_status : variable used to return conversion status
 *          (true)  : if conversion is complete
 *          (false) : if conversion in progress
 * @param _adc : is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType hal_adc_is_conv_done(const adc_config_t *_adc , uint8_t *conv_status)
{
    Std_ReturnType ret_value = E_OK ;
    if((NULL == _adc) || (NULL == conv_status))
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        *conv_status = (uint8_t) (!(ADC_IS_CONV_DONE()));
    }
    return ret_value ;
    
}
/**
 * @brief gets conversion result
 * 
 * @preconditions hal_adc_init , hal_adc_is_conv_done , hal_adc_start_conv
 * 
 * @param result : used to return conversion result
 * @param _adc : is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */

Std_ReturnType hal_adc_get_result(const adc_config_t *_adc , uint16_t *result)
{
    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == _adc) || (NULL == result))
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        switch(_adc->result_format)
        {
            case ADC_RIGHT_JUS:
                *result =  (uint16_t) (ADRESL + (ADRESH << 8));
                break;
            case ADC_LEFT_JUS:

                *result = (uint16_t) ( (ADRESL + (ADRESH << 8)) >> 6 );
                break;
            default:
                ret_value = E_NOT_OK ;
        }
    }
    return ret_value ;
    
}
/**
 * @brief performs the whole conversion then returns the result
 * 
 * @preconditions hal_adc_init
 * 
 * @param channel : channel used in conversion
 * @param result : result from conversion
 * @param _adc : is ADC configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */

Std_ReturnType hal_adc_get_conversion_blocking(const adc_config_t *_adc ,adc_channel_t channel , uint16_t *result )
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_conversion_status = ADC_CONVERSION_IN_PROGRESS;
    
    if((NULL == _adc) || (NULL == result))
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
        ret_value = hal_adc_select_ch(_adc , channel);
        ret_value &= hal_adc_start_conv(_adc);
        
        ret_value &= hal_adc_is_conv_done(_adc , &l_conversion_status);
        
        /* Blocking CODE */
        while(ADCON0bits.DONE);
        ret_value &= hal_adc_get_result(_adc , result);
    }
    return ret_value ;
    
}


/**
 * @brief performs the whole conversion then returns the result
 * 
 * @preconditions hal_adc_init
 * 
 * @param channel : channel used in conversion
 * @param result : result from conversion
 * @param _adc : is ADC configuration object
 * @param conv_status : 
 *          (conversion_started)     : means your conversion started
 *          (conversion_can't_start) : means there is a conversion still in progress
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_adc_get_conversion_interrupt(const adc_config_t *_adc ,adc_channel_t channel)
{
    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == _adc))
    {
        ret_value = E_NOT_OK ;
    }
    else 
    {
            ret_value = hal_adc_select_ch(_adc , channel);
            ret_value &= hal_adc_start_conv(_adc);
    }
    return ret_value ;
    
}


static inline Std_ReturnType adc_input_channel_port_configuration(adc_channel_t adc_channel)
{
    Std_ReturnType ret_value = E_OK ;
    
//    pin_config_t l_channel_pin ;
    
    switch(adc_channel)
    {
        case ADC_CHANNEL_AN0:
//            l_channel_pin.pin = PIN0 ;
//            l_channel_pin.port = PORTA_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            
            SET_BIT(TRISA ,PIN0 );
            break;
        case ADC_CHANNEL_AN1:
//            l_channel_pin.pin = PIN1 ;
//            l_channel_pin.port = PORTA_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);

            SET_BIT(TRISA ,PIN1 );

            break;
        case ADC_CHANNEL_AN2:
//            l_channel_pin.pin = PIN2 ;
//            l_channel_pin.port = PORTA_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISA ,PIN2 );


            break;
        case ADC_CHANNEL_AN3:
//            l_channel_pin.pin = PIN3 ;
//            l_channel_pin.port = PORTA_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISA, PIN3 );
            
            break;
        case ADC_CHANNEL_AN4:
//            l_channel_pin.pin = PIN5 ;
//            l_channel_pin.port = PORTA_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISA, PIN5 );
            ADC_ANALOG_DIGITAL_PORT_CONFIG(ADC_AN4_ANALOG_INPUT);

            
            break;
        case ADC_CHANNEL_AN5:
//            l_channel_pin.pin = PIN0 ;
//            l_channel_pin.port = PORTE_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISE, PIN0 );
            

            break;
        case ADC_CHANNEL_AN6:
//            l_channel_pin.pin = PIN1 ;
//            l_channel_pin.port = PORTE_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISE, PIN1 );

            break;
        case ADC_CHANNEL_AN7:
//            l_channel_pin.pin = PIN2 ;
//            l_channel_pin.port = PORTE_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISE, PIN2 );

            break;
        case ADC_CHANNEL_AN8:
//            l_channel_pin.pin = PIN2 ;
//            l_channel_pin.port = PORTB_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISB, PIN2 );

            break;
        case ADC_CHANNEL_AN9:
//            l_channel_pin.pin = PIN3 ;
//            l_channel_pin.port = PORTB_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISB, PIN3 );

            break;
        case ADC_CHANNEL_AN10:
//            l_channel_pin.pin = PIN1 ;
//            l_channel_pin.port = PORTB_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISB, PIN1 );

            break;
        case ADC_CHANNEL_AN11:
//            l_channel_pin.pin = PIN4 ;
//            l_channel_pin.port = PORTB_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISB, PIN4 );

            break;
        case ADC_CHANNEL_AN12:
//            l_channel_pin.pin = PIN0 ;
//            l_channel_pin.port = PORTB_INDEX ;
//            l_channel_pin.direction = INPUT ;
//            ret_value = gpio_pin_direction_init(&l_channel_pin);
            SET_BIT(TRISB, PIN0 );
            break;
        default:
            ret_value = E_NOT_OK ;
    }
    
    return ret_value ;
}

static inline Std_ReturnType select_result_format(const adc_config_t *_adc)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(ADC_RIGHT_JUS == _adc->result_format)
    {
        ADC_SET_RES_FORMAT_RIGHT_JUS();
    }
    else if(ADC_LEFT_JUS == _adc->result_format)
    {
        ADC_SET_RES_FORMAT_LEFT_JUS();
    }
    else { ret_value = E_NOT_OK; }

    return ret_value ;

}

static inline Std_ReturnType select_ref_voltage(const adc_config_t *_adc)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(ADC_VOLTAGE_REF_EN == _adc->voltage_ref)
    {
        ADC_SET_VREF_TO_EXT_SOURCE();
    }
    else if(ADC_VOLTAGE_REF_DIS == _adc->voltage_ref)
    {
        ADC_SET_VREF_TO_VDD();
    }
    else { ret_value = E_NOT_OK; }


    return ret_value ;
}


void __attribute__ ((used)) ADC_ISR(void)
{
    /* clear flag */
    INT_ADC_CLEAR_FLAG();
    /* some code */
    
    /* call callback */
    if(NULL != adc_InterruptHandler)
    {
        adc_InterruptHandler();
    }
}



