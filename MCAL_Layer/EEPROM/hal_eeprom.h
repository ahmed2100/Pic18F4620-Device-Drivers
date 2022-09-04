/* 
 * File:   hal_eeprom.h
 * Author: ahmed
 *
 * Created on August 6, 2022, 4:18 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H


/* Includes */

#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../mcal_std_types.h"
#include "../Interrupts/mcal_interrupt_config.h"

/* Macro Defines */

/* Macro Functions */
#define EEPROM_EEPGD_CLEAR()                        (EECON1bits.EEPGD = 0 ) 
#define EEPROM_CFGS_CLEAR()                         (EECON1bits.CFGS = 0  ) 
#define EEPROM_WR_INITIATE()                        (EECON1bits.WR = 1    ) 
#define EEPROM_WREN_DIS()                           (EECON1bits.WREN = 0  ) 
#define EEPROM_WREN_EN()                            (EECON1bits.WREN = 1  ) 
#define EEPROM_RD_INITIATE()                        (EECON1bits.RD = 1    )
#define EEPROM_EEDATA_FILL(DATA)                    (EEDATA = DATA        )

#define EEPROM_ADDR_SET_HIGH(ADDRESS_HIGH)          (EEADRH = ADDRESS_HIGH)
#define EEPROM_ADDR_SET(ADDRESS)                    (EEADR  = ADDRESS     )

#define EEPROM_EECON2_SEND_SEQ(SEQ)                 (EECON2 = SEQ         )
    

/* User-Defined Data Type Declaration */


/* Software Interfaces (Prototypes) */
Std_ReturnType hal_eeprom_read_byte(uint16_t eeprom_address, uint8_t *data);
Std_ReturnType hal_eeprom_write_byte(uint16_t eeprom_address, uint8_t data);
/* Global Shared Variables */



#endif	/* HAL_EEPROM_H */

