/* 
 * File:   hal_adc.h
 * Author: ahmed
 *
 * Created on August 9, 2022, 1:06 PM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H


/* Includes */
#include "../mcal_std_types.h"
#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "hal_adc_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupts/mcal_internal_interrupt.h"

/* Macro Defines */
/**
* @brief A/D Port Configuration Control bits
*
* @note if ADC_AN5_ANALOG_INPUT is selected it means
* that AN5 , AN4 , AN3 , AN2 , AN1 , AN0 are all working as
* analog input channels for ADC  
*/
#define ADC_AN0_ANALOG_INPUT        0x0E
#define ADC_AN1_ANALOG_INPUT        0x0D
#define ADC_AN2_ANALOG_INPUT        0x0C
#define ADC_AN3_ANALOG_INPUT        0x0B
#define ADC_AN4_ANALOG_INPUT        0x0A
#define ADC_AN5_ANALOG_INPUT        0x09
#define ADC_AN6_ANALOG_INPUT        0x08
#define ADC_AN7_ANALOG_INPUT        0x07
#define ADC_AN8_ANALOG_INPUT        0x06
#define ADC_AN9_ANALOG_INPUT        0x05
#define ADC_AN10_ANALOG_INPUT       0x04
#define ADC_AN11_ANALOG_INPUT       0x03
#define ADC_AN12_ANALOG_INPUT       0x02
#define ADC_ALL_ANALOG_INPUT        0x00
#define ADC_ALL_DIGITAL             0x0F

#define ADC_RIGHT_JUS               0x01U
#define ADC_LEFT_JUS                0x00U

#define ADC_VOLTAGE_REF_EN          0x01U
#define ADC_VOLTAGE_REF_DIS         0x00U

#define ADC_CONVERSION_DONE         0x00U
#define ADC_CONVERSION_IN_PROGRESS  0x01U

#define ADC_CONVERSION_STARTED      0x01U
#define ADC_CONVERSION_CANT_START   0x01U

 


/* Macro Functions */

#define ADC_ENABLE()                        (ADCON0bits.ADON = 1      )
#define ADC_DISABLE()                       (ADCON0bits.ADON = 0      )

#define ADC_CONV_START()                    (ADCON0bits.GO = 1        )
#define ADC_IS_CONV_DONE()                  (ADCON0bits.DONE == 1     )

/**
* @brief Sets A/D Port Configuration Control bits
*
* @ref A/D Port Configuration Control bits 
*/
#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_CONFIG)    (ADCON1bits.PCFG = _CONFIG)

#define ADC_SET_VREF_TO_VDD()               do{ ADCON1bits.VCFG0 = 0; ADCON1bits.VCFG1 = 0;}while(0)
                                                  

#define ADC_SET_VREF_TO_EXT_SOURCE()        do{ ADCON1bits.VCFG0 = 1; ADCON1bits.VCFG1 = 1;}while(0)


#define ADC_SET_RES_FORMAT_RIGHT_JUS()      (ADCON2bits.ADFM = 1          )
#define ADC_SET_RES_FORMAT_LEFT_JUS()       (ADCON2bits.ADFM = 0          )

#define ADC_SET_ACQ_TIME(_TIME)             (ADCON2bits.ACQT = _TIME      )

#define ADC_SET_CLOCK(_CLK_SELECT)          (ADCON2bits.ADCS = _CLK_SELECT)

#define ADC_SELECT_CHANNEL(_CH_SELECT)      (ADCON0bits.CHS  = _CH_SELECT )



/* User-Defined Data Type Declaration */

typedef enum {
    ADC_CHANNEL_AN0 = 0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12
}adc_channel_t;


/**
* @brief A/D Acquisition Time Select
*  
*
* @note T(ad) is calculated by using ADC clock freq
*/
typedef enum {
    ADC_ACQ_AUTO = 0,
    ADC_ACQ_2_TAD,
    ADC_ACQ_4_TAD,
    ADC_ACQ_6_TAD,
    ADC_ACQ_8_TAD,
    ADC_ACQ_12_TAD,
    ADC_ACQ_16_TAD,
    ADC_ACQ_20_TAD
} adc_acquisition_t;

/**
* @brief A/D Conversion Clock Select bits
* 
*
*  
*/
typedef enum {
    ADC_CLOCK_FOSC_BY_2 = 0,       
    ADC_CLOCK_FOSC_BY_8,        
    ADC_CLOCK_FOSC_BY_32,       
    ADC_CLOCK_FRC,              
    ADC_CLOCK_FOSC_BY_4,        
    ADC_CLOCK_FOSC_BY_16,       
    ADC_CLOCK_FOSC_BY_64      
}adc_clock_select_t;

typedef struct {
#if ADC_INT_ENABLE==FEATURE_ENABLE
    InterruptHandler ADC_InterruptHandler;
    #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        interrupt_pri_t priority ;
    #endif
#endif
    adc_acquisition_t aqcuisition_time ;   /* @ref adc_acquisition_t  */
    adc_clock_select_t conversion_clock ;   /* @ref adc_clock_select_t  */
    adc_channel_t adc_channel ;             /* @ref adc_channel_t  */
    uint8_t voltage_ref   : 1 ;
    uint8_t result_format : 1 ;
    uint8_t adc_reserved : 5 ;
    
}adc_config_t;

/* Software Interfaces (Prototypes) */
Std_ReturnType hal_adc_init(const adc_config_t *_adc);
Std_ReturnType hal_adc_de_init(const adc_config_t *_adc);
Std_ReturnType hal_adc_select_ch(const adc_config_t *_adc , adc_channel_t channel);
Std_ReturnType hal_adc_start_conv(const adc_config_t *_adc);
Std_ReturnType hal_adc_is_conv_done(const adc_config_t *_adc , uint8_t *conv_status);
Std_ReturnType hal_adc_get_result(const adc_config_t *_adc , uint16_t *result);
Std_ReturnType hal_adc_get_conversion_blocking(const adc_config_t *_adc ,adc_channel_t channel , uint16_t *result );
Std_ReturnType hal_adc_get_conversion_interrupt(const adc_config_t *_adc ,adc_channel_t channel);

/* Global Shared Variables */



#endif	/* HAL_ADC_H */

