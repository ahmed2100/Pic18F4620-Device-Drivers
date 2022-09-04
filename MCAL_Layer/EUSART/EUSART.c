/* 
 * File:   EUSART.c
 * Author: ahmed
 *
 * Created on August 24, 2022, 11:25 AM
 */

#include "EUSART.h"



#if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler eusart_Tx_InterruptHandler = NULL ;
#endif
    
#if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
    static InterruptHandler eusart_Rx_InterruptHandler = NULL ;
    static InterruptHandler eusart_overrunerr_CallBack = NULL ;
    static InterruptHandler eusart_frameerr_CallBack   = NULL ;
#endif
    
static inline Std_ReturnType async_config_baud_rate_gen(const uart_config_t *_eusart_obj);    

static inline Std_ReturnType async_Tx_config(const uart_tx_config_t *_tx_obj );
static inline Std_ReturnType async_Rx_config(const uart_rx_config_t *_rx_obj );
static inline Std_ReturnType Tx_config_interrupt(const uart_tx_config_t *_tx_obj);
static inline Std_ReturnType Rx_config_interrupt(const uart_rx_config_t *_rx_obj);

static inline uint8_t calc_parity_odd(uint8_t data);
static inline uint8_t calc_parity_even(uint8_t data);


#if EUSART_OPERATION_MODE_SELECTED==EUSART_ASYNCHRONOUS_MODE
/**
 * @brief initializes EUSART module in async mode
 * 
 * @param _eusart_obj configuration object for async. mode
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_init(const uart_config_t *_eusart_obj)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _eusart_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Disable Module  */
        EUSART_DISABLE();
        
        /* Calculate baud rate register and write it */
        ret_val = async_config_baud_rate_gen(_eusart_obj);
        
        /* Enable Async. mode */
        EUSART_ASYNC_MODE();
        
        /* Configure Tx pin */
        TRISCbits.RC6 = GPIO_HIGH ;
        /* Configure Tx */
        ret_val &= async_Tx_config(&(_eusart_obj->tx_config));
        
        /* Configure Rx pin */
        TRISCbits.RC7 = GPIO_HIGH ;
        /* Configure Rx */
        ret_val &= async_Rx_config(&(_eusart_obj->rx_config));

        /* Enable Module */
        EUSART_ENABLE();    
    }
    
    return ret_val ;
    
    
}

/**
 * @brief disables eusart module
 * 
 * @param _eusart_obj configuration object for async. mode
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_deinit(const uart_config_t *_eusart_obj)
{
    
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _eusart_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* Disable Tx Interrupt */
        #if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
            INT_EUSART_Tx_DISABLE();
        #endif
        /* Disable Rx Interrupt */
        #if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
            INT_EUSART_Rx_DISABLE();
        #endif
        /* Disable EUSART module */
            EUSART_DISABLE();
    }
    
    return ret_val ;
    
}

/**
 * @brief transmits a 8bit or 9bit data
 * 
 * @param _eusart_obj configuration object for async. mode
 * @param data 9bit data that contains an extra 9th bit for parity , address , or data 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_transmit_data(const uart_config_t *_eusart_obj , uint16_t data)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _eusart_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        /* If 9th bit exists */
        if(EUSART_ASYNCH_Tx_BIT_9_ENABLED == _eusart_obj->tx_config.tx_9th_bit_en)
        {
             
            /* Calc 9th bit */
            if(EUSART_ASYNCH_BIT_9_DATA == _eusart_obj->tx_config.uart_tx_9th_bit_role)
            {
                /* If role is DATA */
                TXSTAbits.TX9D = (__bit) (data >> 8);
                
            }
            else if(EUSART_ASYNCH_BIT_9_PARI_EVEN == _eusart_obj->tx_config.uart_tx_9th_bit_role)
            {
                /* else if role is PARI_EVEN */
                TXSTAbits.TX9D = (__bit) calc_parity_even( (uint8_t) data);

            }
            else if (EUSART_ASYNCH_BIT_9_PARI_ODD == _eusart_obj->tx_config.uart_tx_9th_bit_role)
            {
                /* else if role is PARI_ODD */
                TXSTAbits.TX9D = (__bit) calc_parity_odd( (uint8_t) data);

            }

        }
        /* load data into TXREG */
        TXREG = data ;
#if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
        PIE1bits.TXIE = INT_ENABLE ;
#endif
    }
    
    return ret_val ;
    
    
}

/**
 * @brief this interface is responsible for reading data (8bit or 9bit) ( it does not handle errors.)
 * errors are handled in the interrupt callback or in using an interface to check errors.
 * 
 * @param _eusart_obj configuration object for async. mode
 * @param data pointer to variable to return data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_async_read_data(const uart_config_t *_eusart_obj , uint16_t *data)
{
    Std_ReturnType ret_val = E_OK ;
    uint16_t l_temp_data  = 0;
    
    if((NULL == _eusart_obj) || (NULL == data))
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        if(1 == PIR1bits.RCIF)
        {
            if(EUSART_ASYNCH_Rx_BIT_9_ENABLED == _eusart_obj->rx_config.rx_9th_bit_en)
            {
                /* Read 9th bit */
                if(EUSART_ASYNCH_BIT_9_DATA == _eusart_obj->rx_config.uart_rx_9th_bit_role)
                {
                    /* Data */
                l_temp_data = RCREG ;
                l_temp_data +=  ((RX9D == GPIO_HIGH) ? (0x0001 << 8) : 0x0000) ;

                *data = l_temp_data ;

                }
                else if(EUSART_ASYNCH_BIT_9_PARI_EVEN == _eusart_obj->rx_config.uart_rx_9th_bit_role)
                {  
                    /* Parity odd */
                    /* Load Data */
                    l_temp_data = RCREG ;
                    /* Store RX9D */
                    l_temp_data +=  ((RCSTAbits.RX9D == GPIO_HIGH) ? (0x0001 << 8) : 0x0000) ;

                    /* Calculate Parity */
                    if( calc_parity_even( (uint8_t) l_temp_data) == (l_temp_data >> 8))
                    {
                        /* Parity right  */
                        *data = (l_temp_data & 0x00FF) ;

                    }
                    else 
                    {
                        /* Parity wrong */
                        /* execute callback */
               }



                }
                else if (EUSART_ASYNCH_BIT_9_PARI_ODD == _eusart_obj->rx_config.uart_rx_9th_bit_role)
                {
                    /* Parity odd */
                    /* Load Data */
                    l_temp_data = RCREG ;
                    /* Store RX9D */
                    l_temp_data +=  ((RCSTAbits.RX9D == GPIO_HIGH) ? (0x0001 << 8) : 0x0000) ;

                    /* Calculate Parity */

                    /* Compare Parity */
                    if( calc_parity_odd( (uint8_t) l_temp_data) == (l_temp_data >> 8) )
                    {
                        /* Parity right  */
                        *data = (l_temp_data & 0x00FF) ;

                    }
                    else 
                    {
                        /* Parity wrong     */
                        /* execute callback */

                    }

                }
                else { /* Nothing */ }
            }
            else if(EUSART_ASYNCH_Rx_BIT_9_DISABLED == _eusart_obj->rx_config.rx_9th_bit_en) 
            {
                *data = RCREG ;
            }
            else { /* Nothing */ }
        }
        else 
        {
            ret_val = E_NOT_OK ;
        }
        
    }
    
    return ret_val ;

}

/**
 * 
 * @param _eusart_obj
 * @param error
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
Std_ReturnType eusart_asynch_check_for_errors(void)
{
    Std_ReturnType ret_val = E_OK ;
    /* Check for Overrun error */
    if( EUSART_ASYNCH_ERROR_OCCURED == RCSTA1bits.OERR )
    {
        /* Clear error */
        EUSART_ASYNCH_Rx_DIS(); 
        EUSART_ASYNCH_Rx_EN();
        if(eusart_overrunerr_CallBack)
        {
            eusart_overrunerr_CallBack();
        }  
    }
    /* Check for Frame error */
    if( EUSART_ASYNCH_ERROR_OCCURED == RCSTA1bits.FERR )
    {
        /* Clear error */
        /* Dummy read */
        while(!(RCREG));
        if(eusart_frameerr_CallBack)
        {
            eusart_frameerr_CallBack();
        }
    }
    
    return ret_val ;
}

Std_ReturnType eusart_async_transmit_data_blocking(const uart_config_t *_eusart_obj , uint16_t data)
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _eusart_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        
        while(!(TXSTAbits.TRMT));
        eusart_async_transmit_data(_eusart_obj , data);
        
    }
    
    return ret_val ;
}
Std_ReturnType eusart_async_read_data_blocking(const uart_config_t *_eusart_obj , uint16_t *data)
{
    Std_ReturnType ret_val = E_OK ;
    
    if((NULL == _eusart_obj) || (NULL == data))
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        while(!(PIR1bits.RCIF));
        eusart_async_read_data(_eusart_obj , data);
        
    }
    
    return ret_val ;
}

Std_ReturnType eusart_async_transmit_data_string_blocking(const uart_config_t *_eusart_obj , uint8_t *data , uint16_t len)
{
    Std_ReturnType ret_val = E_OK ;
    uint16_t l_counter = 0 ;
    
    if((NULL == _eusart_obj) || (NULL == data))
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        for(l_counter = 0 ; l_counter < len ;l_counter++)
        {
            eusart_async_transmit_data_blocking(_eusart_obj , data[l_counter]);
        }
        
    }
    
    return ret_val ;
    
}


#endif /* Async Mode  */

static Std_ReturnType async_Tx_config(const uart_tx_config_t *_tx_obj )
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _tx_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        if(EUSART_ASYNCH_TX_EN ==  _tx_obj->tx_enable)
        {
            
            /* Disable Tx */
            EUSART_ASYNCH_Tx_DIS();
            /* Configure Tx interrupt */
            ret_val = Tx_config_interrupt(_tx_obj);
            /* Configure 8bit or 9bit Tx */
            if(EUSART_ASYNCH_Tx_BIT_9_ENABLED == _tx_obj->tx_9th_bit_en )
            {
                TXSTAbits.TX9 = EUSART_ASYNCH_Tx_BIT_9_ENABLED ;
            }
            else if (EUSART_ASYNCH_Tx_BIT_9_DISABLED == _tx_obj->tx_9th_bit_en )
            {
                TXSTAbits.TX9 = EUSART_ASYNCH_Tx_BIT_9_DISABLED ;
            }
            else {}

            /* Enable Tx */
            EUSART_ASYNCH_Tx_EN();
        }
        else if (EUSART_ASYNCH_TX_DIS ==  _tx_obj->tx_enable)
        {
            /* Disable Tx */
            EUSART_ASYNCH_Tx_DIS();
        }
        else {}
    }

    
    return ret_val ;
    
}
static Std_ReturnType async_Rx_config(const uart_rx_config_t *_rx_obj )
{
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _rx_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        if(EUSART_ASYNCH_RX_EN ==  _rx_obj->rx_enable)
        {
            /* Disable Rx */
            EUSART_ASYNCH_Rx_DIS();
            /* Configure Rx interrupt */
            ret_val = Rx_config_interrupt(_rx_obj);
            /* Configure 8bit or 9bit Rx */
            if(EUSART_ASYNCH_Rx_BIT_9_ENABLED == _rx_obj->rx_9th_bit_en )
            {
                RCSTAbits.RX9 = EUSART_ASYNCH_Rx_BIT_9_ENABLED ;
            }
                else if (EUSART_ASYNCH_Rx_BIT_9_DISABLED == _rx_obj->rx_9th_bit_en )
            {
                RCSTAbits.RX9 = EUSART_ASYNCH_Rx_BIT_9_DISABLED ;
            }
            else {}

            /* Enable Rx */
            EUSART_ASYNCH_Rx_EN();
        }
        else if(EUSART_ASYNCH_RX_DIS ==  _rx_obj->rx_enable)
        {
            /* Disable Rx */
            EUSART_ASYNCH_Rx_DIS();
        }
        else {}
        
    }
    
    return ret_val ;
    
}

static inline Std_ReturnType Tx_config_interrupt(const uart_tx_config_t *_tx_obj)
{
    
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _tx_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        #if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
        INT_EUSART_Tx_ENABLE();
        eusart_Tx_InterruptHandler = _tx_obj->tx_InterruptHandler ;
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_PRI_HIGH == _tx_obj->uart_tx_priority)
                {
                    INT_EUSART_Tx_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();

                }
                else if(INT_PRI_LOW == _tx_obj->uart_tx_priority)
                {
                    INT_EUSART_Tx_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else { /* Nothing */ }
            #else
                INT_PRI_FET_DIS();
                INT_PERIPHERAL_EN();
                INT_GLOBAL_EN();
            #endif  
        #endif
        
    }
    
    return ret_val ;
    
    
}
static inline Std_ReturnType Rx_config_interrupt(const uart_rx_config_t *_rx_obj)
{
    
     Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _rx_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        #if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
        INT_EUSART_Rx_ENABLE();
        eusart_Rx_InterruptHandler = _rx_obj->rx_InterruptHandler ;
        eusart_frameerr_CallBack = _rx_obj->ferr_InterruptHandler ;
        eusart_overrunerr_CallBack = _rx_obj->oerr_InterruptHandler ;
            #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
                INT_PRI_FET_EN();
                if(INT_PRI_HIGH == _rx_obj->uart_rx_priority)
                {
                    INT_EUSART_Rx_PRI_SET_HIGH();
                    INT_GLOBAL_HIGH_EN();

                }
                else if(INT_PRI_LOW == _rx_obj->uart_rx_priority)
                {
                    INT_EUSART_Rx_PRI_SET_LOW();
                    INT_GLOBAL_HIGH_EN();
                    INT_GLOBAL_LOW_EN();
                }
                else { /* Nothing */ }
            #else
                INT_PRI_FET_DIS();
                INT_PERIPHERAL_EN();
                INT_GLOBAL_EN();
            #endif  
        #endif
        
    }
    
    return ret_val ;
    
    
}

static inline Std_ReturnType async_config_baud_rate_gen(const uart_config_t *_eusart_obj)
{
       
    Std_ReturnType ret_val = E_OK ;
    
    if(NULL == _eusart_obj)
    {
        ret_val = E_NOT_OK ;
    }
    else 
    {
        switch(_eusart_obj->baud_rate_config)
        {
            case BAUDRATE_ASYNC_8BIT_LOW_SPEED:
                TXSTAbits.BRGH = EUSART_LOW_SPEED_BAUD_RATE ;
                BAUDCONbits.BRG16 = EUSART_8_BIT_BR ;
                break;
            case BAUDRATE_ASYNC_8BIT_HIGH_SPEED:
                TXSTAbits.BRGH = EUSART_HIGH_SPEED_BAUD_RATE ;
                BAUDCONbits.BRG16 = EUSART_8_BIT_BR ;
                break ;
            case BAUDRATE_ASYNC_16BIT_LOW_SPEED :
                TXSTAbits.BRGH = EUSART_LOW_SPEED_BAUD_RATE ;
                BAUDCONbits.BRG16 = EUSART_16_BIT_BR ;
                break ;
            case BAUDRATE_ASYNC_16BIT_HIGH_SPEED:
                TXSTAbits.BRGH = EUSART_HIGH_SPEED_BAUD_RATE ;
                BAUDCONbits.BRG16 = EUSART_16_BIT_BR ;
                break ;
            default:
                ret_val = E_NOT_OK ;
                break ;
        }
        
        /* Set SPBRG:SPBRGH value */
        EUSART_ASYNCH_SET_BRG_NUMBER(_eusart_obj->uart_baud_rate_reg_value);
    }
    
    return ret_val ;
    
}

static inline uint8_t calc_parity_odd(uint8_t data)
{
    /* Ones must be odd so if we get an even we set parity to be 1 */
    
    uint8_t number_of_ones = 0 ;
    uint8_t l_counter = 0 ;
//    
//    while(data != 0)
//    {
//        if(0 != (data % 2))
//        {
//            number_of_ones++ ;
//        }
//        data /= 2 ;    
//    }
//    
//    if((number_of_ones % 2) == 0)
//    {
//        return 1 ;
//    }
//    return 0 ;
    for(l_counter = 0 ; l_counter < 8 ; l_counter++ )
    {
        if( (data & 0x01 << l_counter) != 0 )
        {
            number_of_ones++ ;
        }
    }
    
    if((number_of_ones % 2) == 0) // even
    {
        return 1 ;
    }
    return 0 ;
}
static inline uint8_t calc_parity_even(uint8_t data)
{
    /* Ones must be even so if we get an odd we set parity to be 1 */
    uint8_t number_of_ones = 0 ;
    uint8_t l_counter = 0 ;


//    /* Count number of ones */
//    while(data != 0)
//    {
//        if(0 != (data % 2))
//        {
//            number_of_ones++ ;
//        }
//        data /= 2 ;    
//    }
    
    for(l_counter = 0 ; l_counter < 8 ; l_counter++ )
    {
        if( (data & 0x01 << l_counter) != 0 )
        {
            number_of_ones++ ;
        }
    }
    
    if((number_of_ones % 2) != 0) // odd
    {
        return 1 ;
    }
    return 0 ;
}

void EUSART_Tx_ISR(void)
{
#if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
    /* Some code */
    
    /* disable interrupt to fix the issue of high rate interrupt
     * which results in the callback never being executed  
     */
    PIE1bits.TXIE = INT_DISABLE ;

    if(eusart_Tx_InterruptHandler)
    {
        eusart_Tx_InterruptHandler();
    }
#endif
}

void EUSART_Rx_ISR(void)
{
    #if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
    /* Some code */

    if(eusart_Rx_InterruptHandler)
    {
        eusart_Rx_InterruptHandler();
    }
    
    if( EUSART_ASYNCH_ERROR_OCCURED == RCSTA1bits.OERR )
        {
            /* Clear error */
            EUSART_ASYNCH_Rx_DIS(); 
            EUSART_ASYNCH_Rx_EN();
            if(eusart_overrunerr_CallBack)
            {
                eusart_overrunerr_CallBack();
            }  
        }
        /* Check for Frame error */
        if( EUSART_ASYNCH_ERROR_OCCURED == RCSTA1bits.FERR )
        {
            /* Clear error */
            /* Dummy read */
            while(!(RCREG));
            if(eusart_frameerr_CallBack)
            {
                eusart_frameerr_CallBack();
            }
        }
    
 
    #endif
}
#if EUSART_OPERATION_MODE_SELECTED==EUSART_SYNCHRONOUS_MODE
/* To be implemented */

#endif /* Sync. Mode */
