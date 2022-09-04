/* 
 * File:   hal_ccp2.h
 * Author: ahmed
 *
 * Created on August 18, 2022, 10:58 AM
 */

#ifndef HAL_CCP2_H
#define	HAL_CCP2_H

/******************************************** Includes ********************************************/

#include "../hal_ccp.h"
#include "hal_ccp2_config.h"
#include "../../Timer2/hal_timer2.h"

/******************************************** Macro Defines ***************************************************************/


#define CCP2_MODE_PWM                                   0x0CU

#define CCP2_TMR1                                       0x00U
#define CCP2_TMR3                                       0x01U

#define CCP2_RC1_PIN                                    0x00U
#define CCP2_RB3_PIN                                    0x01U




/******************************************** Macro Functions *************************************************************/
/* Function to reset ccp2 module  */
#define CCP2_RESET()                                    (CCP2CON = 0x00U)

#define CCP2_SET_MODE(_MODE)                           (CCP2CONbits.CCP2M = _MODE)




/******************************************** User-Defined Data Type Declaration ********************************************/

#if CCP2_MODE_SELECTED==CAPTURE_CONFIG_MODE
typedef struct {
    /* Interrupt Handler */
    #if CCP2_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler ccp2_InterruptHandler ;
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
            uint8_t priority : 1 ;
        #endif  
    #endif
    uint8_t ccp2_capture_mode_variant : 4 ;
    uint8_t ccp2_pin : 1 ;
    uint8_t ccp2_reserved : 2 ;
}ccp2_config_t;
#endif

#if CCP2_MODE_SELECTED==COMPARE_CONFIG_MODE
typedef struct {    
    /* Interrupt Handler */
    #if CCP2_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler ccp2_InterruptHandler ;
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
            uint8_t priority : 1 ;
        #endif  
    #endif
    uint8_t ccp2_compare_mode_variant : 4 ;
    uint8_t ccp2_pin : 1 ;
    uint8_t ccp2_reserved : 2 ;

    

}ccp2_config_t;
#endif

#if CCP2_MODE_SELECTED==PWM_CONFIG_MODE
typedef struct {  
    uint32_t PWM_freq ;
    timer2_config_t timer2 ;
    #if CCP2_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler ccp2_InterruptHandler ;
    #endif
    uint8_t pre_scaler ;
    uint8_t duty_cycle ;
    #if CCP2_INT_ENABLE==FEATURE_ENABLE
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
                uint8_t priority : 1 ;
        #endif  
    #endif
    uint8_t ccp2_pin : 1 ;

}ccp2_config_t;
#endif


/******************************************** Software Interfaces (Prototypes) ********************************************/
Std_ReturnType hal_ccp2_deinit(const ccp2_config_t *_ccp2_config);


#if CCP2_MODE_SELECTED==CAPTURE_CONFIG_MODE
/*------------------------------------------- CAPTURE MODE ------------------------------------------------------------*/
Std_ReturnType hal_ccp2_capture_mode_init(const ccp2_config_t *_ccp2_config);
Std_ReturnType hal_ccp2_capture_mode_read(const ccp2_config_t *_ccp2_config , uint16_t *result);
Std_ReturnType hal_ccp2_capture_mode_change_variant(const ccp2_config_t *_ccp2_config , uint8_t _variant);

#endif
#if CCP2_MODE_SELECTED==COMPARE_CONFIG_MODE
/*------------------------------------------- COMPARE MODE ------------------------------------------------------------*/
Std_ReturnType hal_ccp2_compare_mode_init(const ccp2_config_t *_ccp2_config);
Std_ReturnType hal_ccp2_compare_mode_set_value(const ccp2_config_t *_ccp2_config , uint16_t val);
Std_ReturnType hal_ccp2_compare_mode_change_variant(const ccp2_config_t *_ccp2_config , uint8_t variant);
#endif
#if CCP2_MODE_SELECTED==PWM_CONFIG_MODE
/*------------------------------------------- PWM MODE ------------------------------------------------------------*/
Std_ReturnType hal_ccp2_PWM_mode_init(ccp2_config_t *_ccp2_config);
Std_ReturnType hal_ccp2_PWM_mode_change_DC(const ccp2_config_t *_ccp2_config , uint8_t _DC);
Std_ReturnType hal_ccp2_PWM_mode_start_PWM(const ccp2_config_t *_ccp2_config);
Std_ReturnType hal_ccp2_PWM_mode_stop(const ccp2_config_t *_ccp2_config);

#endif







/******************************************** Global Shared Variables ********************************************/


#endif	/* HAL_CCP2_H */

