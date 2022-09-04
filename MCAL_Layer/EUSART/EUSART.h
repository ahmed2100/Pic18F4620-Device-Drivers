/* 
 * File:   EUSART.h
 * Author: ahmed
 *
 * Created on August 24, 2022, 11:27 AM
 */

#ifndef EUSART_H
#define	EUSART_H


/******************************************** Includes ********************************************/

#include "C:\Program Files\Microchip\xc8\v2.36\pic\include\pic18.h"
#include "../Interrupts/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "EUSART_config.h"



/******************************************** Macro Defines ***************************************************************/




/* Async Macro Defines */
/* Enables and disables transimition in async. mode */
#define EUSART_ASYNCH_TX_EN                1
#define EUSART_ASYNCH_TX_DIS               0

/* Enables and disables reception in async. mode */
#define EUSART_ASYNCH_RX_EN                1
#define EUSART_ASYNCH_RX_DIS               0

/* Baud rate Macros */
#define EUSART_HIGH_SPEED_BAUD_RATE        1
#define EUSART_LOW_SPEED_BAUD_RATE         0

#define EUSART_16_BIT_BR                   1
#define EUSART_8_BIT_BR                    0


/* 9th bit role */
#define EUSART_ASYNCH_BIT_9_DATA           0
#define EUSART_ASYNCH_BIT_9_PARI_EVEN      1
#define EUSART_ASYNCH_BIT_9_PARI_ODD       2

/* 9th bit transimition enabled or disabled */
#define EUSART_ASYNCH_Tx_BIT_9_DISABLED       0
#define EUSART_ASYNCH_Tx_BIT_9_ENABLED        1

/* 9th bit reception enabled or disabled */
#define EUSART_ASYNCH_Rx_BIT_9_DISABLED       0
#define EUSART_ASYNCH_Rx_BIT_9_ENABLED        1

/* error occur or not */

#define EUSART_ASYNCH_ERROR_OCCURED           1
#define EUSART_ASYNCH_NO_ERROR                0



/******************************************** Macro Functions *************************************************************/




/* Function to Disable and Enable EUSART Module  */
#define EUSART_ENABLE()                                 (RCSTAbits.SPEN = 1 )
#define EUSART_DISABLE()                                (RCSTAbits.SPEN = 0 )

/* Function to Set Async. mode */
#define EUSART_ASYNC_MODE()                             (TXSTA1bits.SYNC = 0)
/* Function to Configure Tx and Rx pins to input */
#define EUSART_ASYNCH_Tx_Rx_PINS_CONF()                do {TRISCbits.RC6 = 1 ; TRISCbits.RC7 = 1 ;} while(0);
 
/* Function to Set calculated BGR_NUMBER */
#define EUSART_ASYNCH_SET_BRG_NUMBER(_NUM)             SPBRGH = ( (_NUM >> 8)) ; SPBRG = ( (_NUM)) ; 

/* Function to Enable and Disable Transmitting */
#define EUSART_ASYNCH_Tx_EN()                           (TXSTAbits.TXEN = 1)
#define EUSART_ASYNCH_Tx_DIS()                          (TXSTAbits.TXEN = 0)

/* Function to Enable and Disable Recieving */
#define EUSART_ASYNCH_Rx_EN()                           (RCSTAbits.CREN = 1)
#define EUSART_ASYNCH_Rx_DIS()                          (RCSTAbits.CREN = 0)




/******************************************** User-Defined Data Type Declaration ********************************************/

/* Async mode config */
#if EUSART_OPERATION_MODE_SELECTED==EUSART_ASYNCHRONOUS_MODE

typedef enum {
    BAUDRATE_ASYNC_8BIT_LOW_SPEED ,
    BAUDRATE_ASYNC_8BIT_HIGH_SPEED ,
    BAUDRATE_ASYNC_16BIT_LOW_SPEED ,
    BAUDRATE_ASYNC_16BIT_HIGH_SPEED 
    
}uart_baud_rate_config_t;


typedef struct {
#if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
    InterruptHandler tx_InterruptHandler ;
#endif
    uint8_t tx_reserved : 3 ;
    uint8_t uart_tx_9th_bit_role : 2 ;
#if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
    uint8_t uart_tx_priority : 1 ;
#endif
#endif
    uint8_t tx_enable : 1 ;
    uint8_t tx_9th_bit_en : 1 ;

}uart_tx_config_t;


typedef struct {
    
#if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
    InterruptHandler rx_InterruptHandler ;
    
    InterruptHandler oerr_InterruptHandler ;
    InterruptHandler ferr_InterruptHandler ;
#endif
    uint8_t rx_reserved : 3 ;
    
    uint8_t uart_rx_9th_bit_role : 2 ;
#if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
    uint8_t uart_rx_priority : 1 ;
#endif
#endif
    uint8_t rx_enable : 1 ;
    uint8_t rx_9th_bit_en : 1 ;

}uart_rx_config_t;



//typedef union 
//{
//    struct 
//    {
//        uint8_t error_status_reserved : 6 ;
//        uint8_t frame_error : 1 ;
//        uint8_t overrun_error : 1 ;
//    };
//    uint8_t status ;
//}uart_error_status_t;


typedef struct {
    
    uint16_t uart_baud_rate_reg_value ;
    uart_tx_config_t tx_config ;
    uart_rx_config_t rx_config ;
    uart_baud_rate_config_t baud_rate_config ;


}uart_config_t;

#endif /* Async. Mode */

#if EUSART_OPERATION_MODE_SELECTED==EUSART_SYNCHRONOUS_MODE
/* To be implemented */
/* Sync mode config */

#endif /* Sync. Mode */




/******************************************** Software Interfaces (Prototypes) ********************************************/


#if EUSART_OPERATION_MODE_SELECTED==EUSART_ASYNCHRONOUS_MODE
/**
 * @brief initializes EUSART module in async mode
 * 
 * @param _eusart_obj configuration object for async. mode of EUSART modules.
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_init(const uart_config_t *_eusart_obj);

/**
 * @brief disables eusart module
 * 
 * @param _eusart_obj configuration object for async. mode of EUSART modules.
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_deinit(const uart_config_t *_eusart_obj);

/**
 * @brief transmits a 8bit or 9bit data
 * 
 * @param _eusart_obj configuration object for async. mode of EUSART modules.
 * @param data 9bit data that contains an extra 9th bit for parity , address , or data 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_transmit_data(const uart_config_t *_eusart_obj , uint16_t data);

//Std_ReturnType eusart_asynch_tx_get_status(uart_config_t *_eusart_obj , uint8_t *status);

/**
 * @brief this interface checks for errors , executes callback  , and clears errors 
 * @note if Rx interrupt is enabled errors are handled inside EUSART_Rx_ISR and no need to call this
 *       interface.
 * 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_asynch_check_for_errors(void);


/**
 * @brief this interface is responsible for reading data (8bit or 9bit) ( it does not handle errors.)
 * errors are handled in the interrupt callback or in using an interface to check errors.
 * 
 * @param _eusart_obj configuration object for async. mode of EUSART modules.
 * @param data pointer to variable to return data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_read_data(const uart_config_t *_eusart_obj , uint16_t *data);


Std_ReturnType eusart_async_read_data_blocking(const uart_config_t *_eusart_obj , uint16_t *data);

Std_ReturnType eusart_async_transmit_data_blocking(const uart_config_t *_eusart_obj , uint16_t data);

Std_ReturnType eusart_async_transmit_data_string_blocking(const uart_config_t *_eusart_obj , uint8_t *data , uint16_t len);




#endif




/******************************************** Global Shared Variables ********************************************/


#endif	/* EUSART_H */

