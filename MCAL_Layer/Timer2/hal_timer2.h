/* 
 * File:   hal_timer2.h
 * Author: ahmed
 *
 * Created on August 15, 2022, 1:09 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H


/******************************************** Includes ********************************************/

#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"

/******************************************** Macro Defines ********************************************/
#define TMR2_PRE_SCALER_BY_1                    0x00U
#define TMR2_PRE_SCALER_BY_4                    0x01U
#define TMR2_PRE_SCALER_BY_16                   0x10U


/******************************************** Macro Functions ********************************************/

/* Functions to Disable or Enable Timer2 */
#define TMR2_ENABLE()                           (T2CONbits.TMR2ON = 1)  
#define TMR2_DISABLE()                          (T2CONbits.TMR2ON = 0)  

/* Function to Configure Post-Scaler */
#define TMR2_CONFIG_POST_SELECT(_POST)          (T2CONbits.TOUTPS = _POST)

/* Function to Configure Pre-Scaler */
#define TMR2_CONFIG_PRE_SELECT(_PRE)            (T2CONbits.T2CKPS = _PRE)


/******************************************** User-Defined Data Type Declaration ********************************************/

typedef enum {
    TMR2_POST_SCALER_BY_1 = 0,
    TMR2_POST_SCALER_BY_2,
    TMR2_POST_SCALER_BY_3,
    TMR2_POST_SCALER_BY_4,
    TMR2_POST_SCALER_BY_5,
    TMR2_POST_SCALER_BY_6,
    TMR2_POST_SCALER_BY_7,
    TMR2_POST_SCALER_BY_8,
    TMR2_POST_SCALER_BY_9,
    TMR2_POST_SCALER_BY_10,
    TMR2_POST_SCALER_BY_11,
    TMR2_POST_SCALER_BY_12,
    TMR2_POST_SCALER_BY_13,
    TMR2_POST_SCALER_BY_14,
    TMR2_POST_SCALER_BY_15,
    TMR2_POST_SCALER_BY_16,    
}timer2_postscaler_t;

typedef struct {
    #if TIMER2_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler timer2_InterruptHandler ;
    #endif
    uint8_t pre_loaded_value ;
    uint8_t period_value ;
    timer2_postscaler_t post_scalar_value ;
        uint8_t pre_scaler_value : 2 ;
    #if TIMER2_INT_ENABLE==FEATURE_ENABLE
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
            uint8_t priority : 1 ;
        #endif
    #endif       
}timer2_config_t;

/******************************************** Software Interfaces (Prototypes) ********************************************/

Std_ReturnType hal_timer2_init(const timer2_config_t *_tmr2_config);

Std_ReturnType hal_timer2_deinit(const timer2_config_t *_tmr2_config);


Std_ReturnType hal_timer2_read_val(const timer2_config_t *_tmr2_config ,
        uint8_t *result);

Std_ReturnType hal_timer2_write_val(const timer2_config_t *_tmr2_config ,
        uint8_t val);

/******************************************** Global Shared Variables ********************************************/


#endif	/* HAL_TIMER2_H */

