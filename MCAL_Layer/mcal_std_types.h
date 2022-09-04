/* 
 * File:   mcal_std_types.h
 * Author: ahmed
 *
 * Created on July 11, 2022, 11:15 PM
 */

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H



/* Includes */
#include "std_libraries.h"
#include "compiler.h"

/* User-Defined Data Type Declaration */

typedef uint8_t Std_ReturnType;


/* Macro Defines */

#define STD_HIGH         0x01
#define STD_LOW          0x00

/* for enabling certain implementation */


#define STD_ON           0x01
#define STD_OFF          0x00

#define STD_ACTIVE       0x01
#define STD_IDLE         0x00

#define E_OK             (Std_ReturnType) 0x01
#define E_NOT_OK         (Std_ReturnType) 0x00

/* Macro Functions */


/* Software Interfaces (Prototypes) */

/* Global Shared Variables */

#endif	/* MCAL_STD_TYPES_H */

