/* 
 * File:   hal_timer3.h
 * Author: ahmed
 *
 * Created on August 16, 2022, 11:25 AM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/******************************************** Includes ********************************************/

#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"

/******************************************** Macro Defines ********************************************/

#define TMR3_COUNTER_MODE               0x01U
#define TMR3_TIMER_MODE                 0x00U

#define TMR3_SYNCH_DISABLED             0x01U
#define TMR3_SYNCH_ENABLED              0x00U

#define TMR3_8BIT_READ_WRITE_MODE       0x00U
#define TMR3_16BIT_READ_WRITE_MODE      0x01U

/******************************************** Macro Functions ********************************************/

/* Functions to Enable or Disable Timer3 */
#define TMR3_ENABLE()                           (T3CONbits.TMR3ON = 1)
#define TMR3_DISABLE()                          (T3CONbits.TMR3ON = 0)

/* Function to configure operation mode  */
#define TMR3_CONFIG_SET_MODE(_MODE)             (T3CONbits.TMR3CS = _MODE )

/* Function to configure Synchronization */
#define TMR3_CONFIG_SELECT_SYNCH(_SYNCH)        (T3CONbits.T3SYNC = _SYNCH)

/* Function to configure prescaler */
#define TMR3_CONFIG_SELECT_PRESCALER(_PRE)      (T3CONbits.T3CKPS = _PRE  )

/* Function to configure read/write mode */
#define TMR3_CONFIG_SELECT_RD_WR_MODE(_MODE)    (T3CONbits.RD16 = _MODE   )




/******************************************** User-Defined Data Type Declaration ********************************************/

typedef enum {
    TMR3_PRESCALER_BY_1,
    TMR3_PRESCALER_BY_2,
    TMR3_PRESCALER_BY_4,
    TMR3_PRESCALER_BY_8
}tmr3_prescaler_t;


typedef struct {
    uint16_t preloaded_value ;
    #if TIMER3_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler timer3_InterruptHandler ;
    #endif
    tmr3_prescaler_t prescalar_value ;
    
    #if TIMER3_INT_ENABLE==FEATURE_ENABLE
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
            uint8_t priority : 1 ;
        #endif
    #endif
    uint8_t synchronization  : 1 ;
    uint8_t read_write_mode  : 1 ;
    uint8_t timer_mode       : 1 ; 
    uint8_t timer1_reserved  : 4 ;    
}timer3_config_t;


/******************************************** Software Interfaces (Prototypes) ********************************************/

Std_ReturnType hal_timer3_init(const timer3_config_t *_tmr3_config);

Std_ReturnType hal_timer3_deinit(const timer3_config_t *_tmr3_config);


Std_ReturnType hal_timer3_read_val(const timer3_config_t *_tmr3_config ,
        uint8_t *result);

Std_ReturnType hal_timer3_write_val(const timer3_config_t *_tmr3_config ,
        uint8_t val);

/******************************************** Global Shared Variables ********************************************/

#endif	/* HAL_TIMER3_H */

