/* 
 * File:   hal_timer0.h
 * Author: ahmed
 *
 * Created on August 13, 2022, 7:19 AM
 */

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H

/* Includes */
#include "hal_timer0_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"


/* Macro Defines */

#define TMR0_8BIT                    0x01U
#define TMR0_16BIT                   0x00U

#define TMR0_COUNTER_MODE            0x01U
#define TMR0_TIMER_MODE              0x00U

#define TMR0_PRESCALER_OFF           0x01U
#define TMR0_PRESCALER_ON            0x00U

#define TMR0_EDGE_FALLING            0x01U
#define TMR0_EDGE_RISING             0x00U


/* Macro Functions */
/* functions to enable and disable timer 0 module */
#define TMR0_ENABLE()                       (T0CONbits.TMR0ON = 1 )
#define TMR0_DISABLE()                      (T0CONbits.TMR0ON = 0 )

/* functions to configure timer in 8bit or 16bit modes */
#define TMR0_CONFIG_8BIT()                  (T0CONbits.T08BIT  = 1)
#define TMR0_CONFIG_16BIT()                 (T0CONbits.T016BIT = 0)

/* functions to configure timer in counter or timer mode */
#define TMR0_CONFIG_TIMER_MODE()            (T0CONbits.T0CS = 0   )
#define TMR0_CONFIG_COUNTER_MODE()          (T0CONbits.T0CS = 1   )

/* functions to configure edge when counter mode is selected */
#define TMR0_CONFIG_EDGE_RISING()           (T0CONbits.T0SE = 0   )
#define TMR0_CONFIG_EDGE_FALLING()          (T0CONbits.T0SE = 1   )

/* functions to enable or disable pre-scaler for both modes */    
#define TMR0_CONFIG_PRESCALER_ENABLE()      (T0CONbits.PSA  = 0   )
#define TMR0_CONFIG_PRESCALER_DISABLE()     (T0CONbits.PSA  = 1   )

/* function configures pre-scalar value */
#define TMR0_CONFIG_PRESCALER_SET_VAL(_VAL) (T0CONbits.T0PS   = _VAL )
#define TMR0_CONFIG_SET_MODE(_MODE)         (T0CONbits.T0CS   = _MODE)
#define TMR0_CONFIG_SET_REG_SZ(_SIZE)       (T0CONbits.T08BIT = _SIZE)
#define TMR0_CONFIG_SET_EDGE(_EDGE)         (T0CONbits.T0SE   = _EDGE)


/* User-Defined Data Type Declaration */

typedef enum {
    TMR0_PRESCALER_BY_2 = 0,
    TMR0_PRESCALER_BY_4,
    TMR0_PRESCALER_BY_8,
    TMR0_PRESCALER_BY_16,
    TMR0_PRESCALER_BY_32,
    TMR0_PRESCALER_BY_64,
    TMR0_PRESCALER_BY_128,
    TMR0_PRESCALER_BY_256
}tmr0_prescalar_t;

typedef struct {
    
    tmr0_prescalar_t prescalar_value ;
    uint16_t preloaded_value ;
    
    #if TIMER0_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler timer0_InterruptHandler ;
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
            uint8_t priority : 1 ;
        #endif
    #endif
    
    uint8_t timer_mode       : 1 ;
    uint8_t reg_bit_size     : 1 ;
    uint8_t prescaler_enable : 1 ;
    uint8_t edge             : 1 ;
    uint8_t timer0_reserved  : 3 ;    
}timer0_config_t;

/* Software Interfaces (Prototypes) */
/**
 * @brief this interface initializes timer0 module with the config
 *        specified in _tmr0_config
 * 
 * @param _tmr0_config : configuration to initialize module
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_timer0_init(const timer0_config_t *_tmr0_config);
/**
 * @brief disables timer0 module
 * 
 * @param _tmr0_config
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation 
 */
Std_ReturnType hal_timer0_deinit(const timer0_config_t *_tmr0_config);


Std_ReturnType hal_timer0_read_val(const timer0_config_t *_tmr0_config ,
        uint16_t *result);

Std_ReturnType hal_timer0_write_val(const timer0_config_t *_tmr0_config ,
        uint16_t val);


/* Global Shared Variables */

#endif	/* HAL_TIMER0_H */

