/* 
 * File:   hal_timer1.h
 * Author: ahmed
 *
 * Created on August 14, 2022, 3:41 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/* Includes */
#include "hal_timer1_config.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"

/* Macro Defines */
#define TMR1_COUNTER_MODE               0x01U
#define TMR1_TIMER_MODE                 0x00U

#define TMR1_SYNCH_DISABLED             0x01U
#define TMR1_SYNCH_ENABLED              0x00U

#define TMR1_8BIT_READ_WRITE_MODE       0x00U
#define TMR1_16BIT_READ_WRITE_MODE      0x01U

#define TMR1_OSC_ENABLE                 0x00U
#define TMR1_OSC_DISABLE                0x01U



/******************************************** Macro Functions ********************************************/

/* Functions to Disable and Enable Timer1 */
#define TMR1_ENABLE()                           (T1CONbits.TMR1ON = 1    )
#define TMR1_DISABLE()                          (T1CONbits.TMR1ON = 0    )

/* Functions to Disable and Enable Timer1 OSC */
#define TMR1_CONFIG_OSC_ENABLE()                       (T1CONbits.T1OSCEN = 1   )
#define TMR1_CONFIG_OSC_DISABLE()                      (T1CONbits.T1OSCEN = 0   )

/* Function to configure operation mode  */
#define TMR1_CONFIG_SET_MODE(_MODE)             (T1CONbits.TMR1CS = _MODE)

/* Function to configure Synchronization */

#define TMR1_CONFIG_SELECT_SYNCH(_SYNCH)        (T1CONbits.T1SYNC = _SYNCH)

/* Function to configure prescaler */
#define TMR1_CONFIG_SELECT_PRESCALER(_PRE)      (T1CONbits.T1CKPS = _PRE  )

/* Function to configure read/write mode */

#define TMR1_CONFIG_SELECT_RD_WR_MODE(_MODE)    (T1CONbits.RD16   = _MODE )

/* Function to configure Timer1 OSC */
#define TMR1_CONFIG_OSC(_OSC_CONFIG)            (T1CONbits.T1OSCEN = _OSC_CONFIG)


#define TMR1_SYSTEM_CLK_STATUS()                (T1CONbits.T1RUN)



/******************************************** User-Defined Data Type Declaration ********************************************/

typedef enum {
    TMR1_PRESCALER_BY_1,
    TMR1_PRESCALER_BY_2,
    TMR1_PRESCALER_BY_4,
    TMR1_PRESCALER_BY_8
}tmr1_prescaler_t;


typedef struct {
    tmr1_prescaler_t prescalar_value ;
    uint16_t preloaded_value ;
    
    #if TIMER1_INT_ENABLE==FEATURE_ENABLE
        InterruptHandler timer1_InterruptHandler ;
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
            uint8_t priority : 1 ;
        #endif
    #endif
    uint8_t synchronization  : 1 ;
    uint8_t read_write_mode  : 1 ;
    uint8_t timer_mode       : 1 ;
    uint8_t timer1_osc_config: 1 ;     
    uint8_t timer1_reserved  : 3 ;    
}timer1_config_t;


/******************************************** Software Interfaces (Prototypes) ********************************************/

Std_ReturnType hal_timer1_init(const timer1_config_t *_tmr1_config);

Std_ReturnType hal_timer1_deinit(const timer1_config_t *_tmr1_config);


Std_ReturnType hal_timer1_read_val(const timer1_config_t *_tmr1_config ,
        uint16_t *result);

Std_ReturnType hal_timer1_write_val(const timer1_config_t *_tmr1_config ,
        uint16_t val);


/******************************************** Global Shared Variables ********************************************/











#endif	/* HAL_TIMER1_H */

