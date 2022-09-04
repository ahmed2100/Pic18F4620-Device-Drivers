/* 
 * File:   mcal_interrupt_config.h
 * Author: ahmed
 *
 * Created on August 4, 2022, 10:45 AM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H


/* Includes */
#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../mcal_std_types.h"
#include "mcal_interrupt_gen_config.h"

/* Macro Defines */

#define INT_ENABLE              1
#define INT_DISABLE             0

#define INT_OCCUR               1
#define INT_CLEARED             0


#define INT_PRI_HIGH            1
#define INT_PRI_LOW             0






/* Macro Functions */

/* enable and disable interrupt priority feature */

#define INT_PRI_FET_EN()                     (RCONbits.IPEN = 1  )
#define INT_PRI_FET_DIS()                    (RCONbits.IPEN = 0  )


#define INT_GLOBAL_INTERRUPT_GET_STATE()     (INTCONbits.GIE == 1)
#define INT_GLOBAL_INTERRUPT_SET_STATE(STAT) (INTCONbits.GIE == STAT)
#define INT_GLOBAL_INTERRUPT_DISABLE()       (INTCONbits.GIE = 0 )


#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
/* IPEN = 1 ,enable and disable all global high priority interrupts */
#define INT_GLOBAL_HIGH_EN()                 (INTCONbits.GIEH = 1)
#define INT_GLOBAL_HIGH_DIS()                (INTCONbits.GIEH = 0)

/* IPEN = 1 ,enable and disable all global low priority interrupts */

#define INT_GLOBAL_LOW_EN()                  (INTCONbits.GIEL = 1)
#define INT_GLOBAL_LOW_DIS()                 (INTCONbits.GIEL = 0)

#else
/* IPEN = 0 ,enable and disable all peripheral interrupts */

#define INT_PERIPHERAL_EN()                  (INTCONbits.PEIE = 1)
#define INT_PERIPHERAL_DIS()                 (INTCONbits.PEIE = 0)
/* IPEN = 0 ,enable and disable all global unmasked interrupts */

#define INT_GLOBAL_EN()                      (INTCONbits.GIE = 1 )
#define INT_GLOBAL_DIS()                     (INTCONbits.GIE = 0 )

#endif

/* User-Defined Data Type Declaration */

typedef void ( *InterruptHandler) (void);


typedef enum {
    INT_LOW_PRI,
    INT_HIGH_PRI

} interrupt_pri_t;

/* Software Interfaces (Prototypes) */

/* Global Shared Variables */

#endif	/* MCAL_INTERRUPT_CONFIG_H */

