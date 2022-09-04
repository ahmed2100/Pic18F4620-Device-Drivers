/* 
 * File:   hal_ccp.h
 * Author: ahmed
 *
 * Created on August 18, 2022, 12:39 PM
 */

#ifndef HAL_CCP_GLOBAL_H
#define	HAL_CCP_GLOBAL_H


/******************************************** Includes ********************************************/

#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "./hal_ccp_config.h"


/******************************************** Macro Defines ***************************************************************/


#define CCP_MODE_DISABLE_AND_RESET                     0x00U
#define CCP_MODE_CAPTURE_EVERY_1_FALLING               0x04U
#define CCP_MODE_CAPTURE_EVERY_1_RISING                0x05U
#define CCP_MODE_CAPTURE_EVERY_4_RISING                0x06U
#define CCP_MODE_CAPTURE_EVERY_16_RISING               0x07U

#define CCP_MODE_COMPARE_TOGGLE_MATCH                  0x02U
#define CCP_MODE_COMPARE_TURN_HIGH                     0x08U
#define CCP_MODE_COMPARE_TURN_LOW                      0x09U
#define CCP_MODE_COMPARE_GEN_INT                       0x0AU
#define CCP_MODE_COMPARE_GEN_EVENT                     0x0BU



#define CCP_TMR2_PRE_SCALER_BY_1                       0x01U
#define CCP_TMR2_PRE_SCALER_BY_4                       0x04U
#define CCP_TMR2_PRE_SCALER_BY_16                      0x10U



/******************************************** Macro Functions *************************************************************/

/* Functions to Configure Timer1 and Timer3 for CCP operation */
#define CCP1_CCP2_TMR3()                 do{T3CONbits.T3CCP2 = 1;}while(0);
#define CCP1_TMR1_CCP2_TMR3()   do{T3CONbits.T3CCP2 = 0;T3CONbits.T3CCP1 = 1;}while(0);
#define CCP1_CCP2_TMR1()                 do{T3CONbits.T3CCP2 = 0;T3CONbits.T3CCP1 = 0;}while(0);








/******************************************** User-Defined Data Type Declaration ********************************************/


/******************************************** Software Interfaces (Prototypes) ********************************************/

/******************************************** Global Shared Variables ********************************************/



#endif	/* HAL_CCP_GLOBAL_H */

