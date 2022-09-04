/* 
 * File:   hal_eeprom.c
 * Author: ahmed
 *
 * Created on August 6, 2022, 4:18 PM
 */

#include "hal_eeprom.h"

static Std_ReturnType eeprom_set_address(uint16_t eeprom_address);


Std_ReturnType hal_eeprom_read_byte(uint16_t eeprom_address, uint8_t *data)
{
    Std_ReturnType ret_value = E_OK;
    if(NULL == data)
    {
        ret_value = E_NOT_OK ;
        
    }
    else
    {
        /* Set Address in EEADR and EEADRH */
        eeprom_set_address(eeprom_address);
        /* Clear EEPGD and CFGS */
        EEPROM_EEPGD_CLEAR();
        EEPROM_CFGS_CLEAR();
        /* Initiate Read  */
        EEPROM_RD_INITIATE();
        
        /* Wait for 2 cyles with NOP() */
        NOP();
        NOP();
        /* Return Data */
        *data = EEDATA ;
    }
    
    return ret_value ;
}
Std_ReturnType hal_eeprom_write_byte(uint16_t eeprom_address , uint8_t data)
{
    
    Std_ReturnType ret_value = E_OK;
    uint8_t l_interrupt_state = 0;
    
    /* Set Address in EEADR and EEADRH */
    eeprom_set_address(eeprom_address);
    /* Fill EEDATA  */
    EEPROM_EEDATA_FILL(data);
    /* Clear EEPGD and CFGS */
    EEPROM_EEPGD_CLEAR();
    EEPROM_CFGS_CLEAR();
    /* Enable Writes */
    EEPROM_WREN_EN();
    /* Get Interrupt State and Disable Interrupts */
    l_interrupt_state = INT_GLOBAL_INTERRUPT_GET_STATE();
    INT_GLOBAL_INTERRUPT_DISABLE();
    /* Send Unlocking Sequence to EECON2 */
    /* Send 0x55 */
    EEPROM_EECON2_SEND_SEQ(0x55);
    /* Send 0x0AA */
    EEPROM_EECON2_SEND_SEQ(0x0AA);
    /* Initiate Write */
    EEPROM_WR_INITIATE();
    
    /* wait for Write to finish by polling WR (which is cleared by hardware) */
    while(EECON1bits.WR)
    {
        /* Do Nothing */
    }
    
    /* Return Interrupt State  */
    INT_GLOBAL_INTERRUPT_SET_STATE(l_interrupt_state);
    /* Disable Writes */
    EEPROM_WREN_DIS(); 
    return ret_value ;
    
}

static Std_ReturnType eeprom_set_address(uint16_t eeprom_address)
{
    Std_ReturnType ret_value = E_OK;
    EEPROM_ADDR_SET_HIGH( (uint8_t)((eeprom_address >> 8) & 0x03) );
    
    EEPROM_ADDR_SET( (uint8_t) (eeprom_address & 0xFF)); 

    return ret_value ;
  
}


