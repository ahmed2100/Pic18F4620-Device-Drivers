/* 
 * File:   hal_gpio.c
 * Author: ahmed
 *
 * Created on July 11, 2022, 10:31 PM
 */

/* Includes */
#include "hal_gpio.h"

/* Global Variables */
volatile uint8_t *tris_registers[] = {&TRISA, &TRISB,&TRISC,&TRISD,&TRISE};
volatile uint8_t *lat_registers[] = {&LATA, &LATB,&LATC,&LATD,&LATE};  
volatile uint8_t *port_registers[] = {&PORTA, &PORTB,&PORTC,&PORTD,&PORTE};  


/**
 * @brief Initialize pin direction @ref direction_t 
 * @param _pin_config pointer to pin configurations @ref pin_config_t
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 * 
 */
#if GPIO_PIN_CONFIG==CONFIG_ENABLE    /* used to enable or disable certain functionality */
Std_ReturnType gpio_pin_direction_init(const pin_config_t *_pin_config)
{
    Std_ReturnType ret_value = E_OK;
    if(NULL == _pin_config || 
       _pin_config->pin > PIN_MAX_NUMBER-1 || _pin_config->pin < 0 || 
       _pin_config->port > PORT_MAX_NUMBER-1 || _pin_config->port < 0)
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
       switch(_pin_config->direction)
        {
            case INPUT:
                SET_BIT( *(tris_registers[_pin_config->port]) , _pin_config->pin );
                break;
            case OUTPUT:
                CLEAR_BIT( *(tris_registers[_pin_config->port]) , _pin_config->pin );
                break;
            default:
                ret_value = E_NOT_OK;
        }  
    }
    return ret_value;
}
#endif

/**
 * @brief gets direction status for pin @ref direction_t
 * @param _pin_config pointer to pin configurations @ref pin_config_t
 * @param dir_status
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
#if GPIO_PIN_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config , direction_t *dir_status)
{
    Std_ReturnType ret_value = E_OK;
    
    if(NULL == _pin_config || NULL == dir_status)
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        *dir_status = READ_BIT(*tris_registers[_pin_config->port] , _pin_config->pin);
    }
    
    return ret_value;
    
} // another idea
#endif


/**
 * @brief writes logic to pin @ref logic_t
 * @param _pin_config pointer to pin configurations @ref pin_config_t
 * @param logic
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
#if GPIO_PIN_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config , logic_t logic){
    
    Std_ReturnType ret_value = E_OK;
    
    if(NULL == _pin_config)
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        switch(logic) 
        {
            case GPIO_HIGH:
                SET_BIT(*lat_registers[_pin_config->port] , _pin_config->pin );
                break;
            case GPIO_LOW:
                CLEAR_BIT(*lat_registers[_pin_config->port] , _pin_config->pin );
                
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }
        
    }
    
    return ret_value;
}
#endif

/**
 * @brief reads logic on pin disregarding the direction @ref logic_t
 * @param _pin_config pointer to pin configurations @ref pin_config_t
 * @param logic
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
#if GPIO_PIN_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config , logic_t *logic){

    Std_ReturnType ret_value = E_OK;
    
    if(NULL == _pin_config || NULL == logic)
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        switch (_pin_config->direction) 
        {
            case INPUT:
                *logic = READ_BIT(*port_registers[_pin_config->port] , _pin_config->pin);
                break;
            case OUTPUT:
                *logic = READ_BIT(*lat_registers[_pin_config->port] , _pin_config->pin);
                break;
            default:
                ret_value = E_NOT_OK; 
                break;

        }
    }
    
    return ret_value;
}
#endif

/**
 * @brief toggles logic on pin @ref logic_t
 * @param _pin_config pointer to pin configurations @ref pin_config_t
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
#if GPIO_PIN_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t *_pin_config){

    Std_ReturnType ret_value = E_OK;
    
    if(NULL == _pin_config)
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        TOGGLE_BIT(*lat_registers[_pin_config->port] , _pin_config->pin );
        
    }
    
    return ret_value;
}
#endif

/**
 * @brief initializes direction and logic for pin   @ref logic_t 
 *                                                  @ref direction_t
 * @param _pin_config pointer to pin configurations @ref pin_config_t
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
#if GPIO_PIN_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_pin_init(const pin_config_t *_pin_config)
{
    Std_ReturnType ret_value = E_OK;
    if(NULL == _pin_config)
    {
        ret_value = E_NOT_OK;
    }
    else 
    {
        gpio_pin_direction_init(_pin_config);
        gpio_pin_write_logic(_pin_config , _pin_config->logic);

    }
    return ret_value ;
    
}
#endif


/*****************************************************************************************************************************/

/* Port Configuration */
/**
 * @brief initializes port direction 
 * @param port port index @ref port_index_t
 * @param dir_status uint8_t specifies value written in register
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */
#if GPIO_PORT_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_port_direction_init(port_index_t port , uint8_t dir_status){
    Std_ReturnType ret_value = E_OK;
    if(port > PORT_MAX_NUMBER-1 )
    {
        ret_value = E_NOT_OK;
    }
    else 
    {
        *tris_registers[port] = dir_status;
    }

    
    
    return ret_value;
}
#endif

/**
 * @brief gets direction of a port as uint8
 * @param port port index @ref port_index_t
 * @param dir_status return pointer to return direction of port as uint8 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */

#if GPIO_PORT_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_port_get_direction_status(port_index_t port , uint8_t *dir_status){

    Std_ReturnType ret_value = E_OK;
    
    if( (NULL == dir_status) || (port > PORT_MAX_NUMBER-1))
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        *dir_status = *tris_registers[port];
    }
    
    
    
    return ret_value;
}
#endif

/**
 * @brief write logic to port
 * @param port port index @ref port_index_t 
 * @param logic logic to be written to port
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */

#if GPIO_PORT_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_port_write_logic(port_index_t port , uint8_t logic){

    Std_ReturnType ret_value = E_OK;

    if(port > PORT_MAX_NUMBER-1 )
    {
        ret_value = E_NOT_OK;
    }
    else 
    {
        *lat_registers[port] = logic;
    }
    
    return ret_value;
}
#endif

/**
 * @brief read logic on a port
 * @param port port index @ref port_index_t
 * @param logic pointer to uint8 specifying logic on port 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */

#if GPIO_PORT_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_port_read_logic(port_index_t port , uint8_t *logic){

    Std_ReturnType ret_value = E_OK;
    
    if( (NULL == logic) || (port > PORT_MAX_NUMBER-1))
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        *logic = *port_registers[port];
    }

    return ret_value;
}

#endif
/**
 * @brief toggle logic on a port
 * @param port port index @ref port_index_t
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function had an issue performing the operation
 */

#if GPIO_PORT_CONFIG==CONFIG_ENABLE
Std_ReturnType gpio_port_toggle_logic(port_index_t port){

    Std_ReturnType ret_value = E_OK;

    if( (port > PORT_MAX_NUMBER-1))
    {
        ret_value = E_NOT_OK;
    }
    
    else 
    {
        // *lat_registers[port] = ~(*lat_registers[port]) ;
        *lat_registers[port] ^= PORT_ALL_SET_MASK ;
    }
    
    return ret_value;
}
#endif


