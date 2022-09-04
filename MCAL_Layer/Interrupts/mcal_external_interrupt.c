/* 
 * File:   mcal_external_interrupts.c
 * Author: ahmed
 *
 * Created on August 4, 2022, 10:47 AM
 */

#include "mcal_external_interrupt.h"

static InterruptHandler INT0_InterruptHandler     = NULL ;
static InterruptHandler INT1_InterruptHandler     = NULL ;
static InterruptHandler INT2_InterruptHandler     = NULL ;


static InterruptHandler RB4_InterruptHandler_HIGH = NULL ;
static InterruptHandler RB4_InterruptHandler_LOW  = NULL ;

static InterruptHandler RB5_InterruptHandler_HIGH = NULL ;
static InterruptHandler RB5_InterruptHandler_LOW  = NULL ;

static InterruptHandler RB6_InterruptHandler_HIGH = NULL ;
static InterruptHandler RB6_InterruptHandler_LOW  = NULL ;

static InterruptHandler RB7_InterruptHandler_HIGH = NULL ;
static InterruptHandler RB7_InterruptHandler_LOW  = NULL ;


static Std_ReturnType interrupt_INTx_pin_init(const interrupt_INTx_t *int_obj);
static Std_ReturnType interrupt_INTx_set_edge(const interrupt_INTx_t *int_obj);
static Std_ReturnType interrupt_INTx_set_priority(const interrupt_INTx_t *int_obj);
static Std_ReturnType interrupt_INTx_disable(const interrupt_INTx_t *int_obj);
static Std_ReturnType interrupt_INTx_enable(const interrupt_INTx_t *int_obj);
static Std_ReturnType interrupt_INTx_clear_flag(const interrupt_INTx_t *int_obj);

static Std_ReturnType INT0_set_callback(void ( *InterruptHandler) (void) );
static Std_ReturnType INT1_set_callback(void ( *InterruptHandler) (void) );
static Std_ReturnType INT2_set_callback(void ( *InterruptHandler) (void) );
static Std_ReturnType interrupt_INTx_set_callback(const interrupt_INTx_t *int_obj);



static Std_ReturnType interrupt_RBx_enable(const interrupt_RBx_t *int_obj);
static Std_ReturnType interrupt_RBx_disable(const interrupt_RBx_t *int_obj);
static Std_ReturnType interrupt_RBx_set_priority(const interrupt_RBx_t *int_obj);
static Std_ReturnType interrupt_RBx_pin_init(const interrupt_RBx_t *int_obj);

static Std_ReturnType RB4_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) );
static Std_ReturnType RB5_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) );
static Std_ReturnType RB6_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) );
static Std_ReturnType RB7_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) );

static Std_ReturnType interrupt_RBx_set_callback(const interrupt_RBx_t *int_obj);



void INT0_ISR(void)
{
    /* clear flag */
    INT_INT0_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(INT0_InterruptHandler)
    {
        INT0_InterruptHandler();
    }
}

void INT1_ISR(void)
{
    /* clear flag */
    INT_INT1_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(INT1_InterruptHandler)
    {
        INT1_InterruptHandler();
    }
}

void INT2_ISR(void)
{
    /* clear flag */
    INT_INT2_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(INT2_InterruptHandler)
    {
        INT2_InterruptHandler();
    }
}



void RB4_ISR(uint8_t handler_select)
{
    /* clear flag */
    INT_RBx_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(handler_select)
    {
        if(RB4_InterruptHandler_HIGH)
        {
            RB4_InterruptHandler_HIGH();
        }
    }
    else if(!handler_select)
    {
        if(RB4_InterruptHandler_LOW)
        {
            RB4_InterruptHandler_LOW();
        }
    }
    else {}
    
}
void RB5_ISR(uint8_t handler_select)
{
    /* clear flag */
    INT_RBx_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(handler_select)
    {
        if(RB5_InterruptHandler_HIGH)
        {
            RB5_InterruptHandler_HIGH();
        }
    }
    else if(!handler_select)
    {
        if(RB5_InterruptHandler_LOW)
        {
            RB5_InterruptHandler_LOW();
        }
    }
    else {}
    
}

void RB6_ISR(uint8_t handler_select)
{
    /* clear flag */
    INT_RBx_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(handler_select)
    {
        if(RB6_InterruptHandler_HIGH)
        {
            RB6_InterruptHandler_HIGH();
        }
    }
    else if(!handler_select)
    {
        if(RB6_InterruptHandler_LOW)
        {
            RB6_InterruptHandler_LOW();
        }
    }
    else {}
    
}

void RB7_ISR(uint8_t handler_select)
{
    /* clear flag */
    INT_RBx_CLEAR_FLAG();
    /* mcal layer code */

    /* call callback if exists */
    if(handler_select)
    {
        if(RB7_InterruptHandler_HIGH)
        {
            RB7_InterruptHandler_HIGH();
        }
    }
    else if(!handler_select)
    {
        if(RB7_InterruptHandler_LOW)
        {
            RB7_InterruptHandler_LOW();
        }
    }
    else {}
    
}





Std_ReturnType interrupt_INTx_INIT(const interrupt_INTx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        /* disable interrupt */
        ret_value = interrupt_INTx_disable(int_obj);
        /* clear IF */
        ret_value &= interrupt_INTx_clear_flag(int_obj);

        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        /* Configure priority */
        ret_value &= interrupt_INTx_set_priority(int_obj);
        #else
        INT_PRI_FET_DIS();
        #endif
        /* configure I/O pin */
        ret_value &= interrupt_INTx_pin_init(int_obj);

        /* configure edge */
        ret_value &= interrupt_INTx_set_edge(int_obj);
        /* configure callback function */
        interrupt_INTx_set_callback(int_obj);
        /* re-enable interrupt */
        ret_value &= interrupt_INTx_enable(int_obj);
    }

    return ret_value ;

}





Std_ReturnType interrupt_INTx_DeINIT(const interrupt_INTx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ret_value = interrupt_INTx_disable(int_obj);
    }

    return ret_value ;
    
}

Std_ReturnType interrupt_RBx_INIT(const interrupt_RBx_t *int_obj)
{

    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        /* disable interrupt */
        ret_value = interrupt_RBx_disable(int_obj);
        /* clear IF */
        INT_RBx_CLEAR_FLAG();
        /* configure I/O pin */
        ret_value &= interrupt_RBx_pin_init(int_obj); 
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        /* Configure priority */
        ret_value &= interrupt_RBx_set_priority(int_obj);
        #else
        INT_PRI_FET_DIS();
        #endif
        
        /* configure callback function */
        ret_value &= interrupt_RBx_set_callback(int_obj);
        /* re-enable interrupt */
        ret_value &= interrupt_RBx_enable(int_obj);

    }

    return ret_value ;     
    
}
Std_ReturnType interrupt_RBx_DeINIT(const interrupt_RBx_t *int_obj)
{

    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        interrupt_RBx_disable(int_obj);

    }  

    return ret_value ; 
    
}

static Std_ReturnType interrupt_INTx_pin_init(const interrupt_INTx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        switch(int_obj->src)
        {
            case INTERRUPT_INT0:
                INT_SET_INT0_INPUT();
                break;

            case INTERRUPT_INT1:
                INT_SET_INT1_INPUT();
                break;

            case INTERRUPT_INT2:
                INT_SET_INT2_INPUT();
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }


    }   

    return ret_value ; 

}
static Std_ReturnType interrupt_INTx_set_edge(const interrupt_INTx_t *int_obj)
{

    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        switch(int_obj->src)
        {
            case INTERRUPT_INT0:

                if(INT_RISING_EDGE ==  int_obj->edge)
                {
                    INT_INT0_RISING_EDGE();
                }
                else if (INT_FALLING_EDGE ==  int_obj->edge)
                {
                    INT_INT0_FALLING_EDGE();
                }
                else
                {
                    ret_value = E_NOT_OK ;
                }
                break;

            case INTERRUPT_INT1:
                if(INT_RISING_EDGE ==  int_obj->edge)
                {
                    INT_INT1_RISING_EDGE();
                }
                else if (INT_FALLING_EDGE ==  int_obj->edge)
                {
                    INT_INT1_FALLING_EDGE();
                }
                else
                {
                    ret_value = E_NOT_OK ;
                }
                break;

            case INTERRUPT_INT2:
                if(INT_RISING_EDGE ==  int_obj->edge)
                {
                    INT_INT2_RISING_EDGE();
                }
                else if (INT_FALLING_EDGE ==  int_obj->edge)
                {
                    INT_INT2_FALLING_EDGE();
                }
                else
                {
                    ret_value = E_NOT_OK ;
                }
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }


    }  

    return ret_value ; 
    
    
}
#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
static Std_ReturnType interrupt_INTx_set_priority(const interrupt_INTx_t *int_obj)
{

    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        INT_PRI_FET_EN();

        switch(int_obj->src)
        {
            case INTERRUPT_INT0:
                // fixed priority
                break;

            case INTERRUPT_INT1:
                if(INT_HIGH_PRI ==  int_obj->priority)
                {
                    INT_INT1_PRI_HIGH();
                }
                else if (INT_LOW_PRI ==  int_obj->priority)
                {
                    INT_INT1_PRI_LOW();
                }
                else { ret_value = E_NOT_OK ;}
                break;

            case INTERRUPT_INT2:
                if(INT_HIGH_PRI ==  int_obj->priority)
                {
                    INT_INT2_PRI_HIGH();
                }
                else if (INT_LOW_PRI ==  int_obj->priority)
                {
                    INT_INT2_PRI_LOW();
                }
                else { ret_value = E_NOT_OK ;}
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }
    }  

    return ret_value ;   
}
#endif

static Std_ReturnType interrupt_INTx_disable(const interrupt_INTx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        if( INT_HIGH_PRI == int_obj->priority)
        {
            INT_GLOBAL_HIGH_DIS();
        }
        else if(INT_LOW_PRI == int_obj->priority)
        {
            INT_GLOBAL_LOW_DIS();
        }
        else
        {}
        #else
        INT_GLOBAL_DIS();
        INT_PERIPHERAL_DIS();
        #endif

        switch(int_obj->src)
        {
            case INTERRUPT_INT0:
                INT_INT0_DISABLE();
                break;

            case INTERRUPT_INT1:
                INT_INT1_DISABLE();
                break;

            case INTERRUPT_INT2:
                INT_INT2_DISABLE();
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }


    }  

    return ret_value ; 
    
    
}
static Std_ReturnType interrupt_INTx_enable(const interrupt_INTx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        /* Priority Configuration */
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        if( INT_HIGH_PRI == int_obj->priority)
        {
            INT_GLOBAL_HIGH_EN();
        }
        else if(INT_LOW_PRI == int_obj->priority)
        {
            INT_GLOBAL_LOW_EN();
            INT_GLOBAL_HIGH_EN();

        }
        else
        { /* Do Nothing */ }
        #else
        INT_GLOBAL_EN();
        INT_PERIPHERAL_EN();

        #endif

        switch(int_obj->src)
        {
            case INTERRUPT_INT0:
                INT_INT0_ENABLE();
                break;

            case INTERRUPT_INT1:
                INT_INT1_ENABLE();
                break;

            case INTERRUPT_INT2:
                INT_INT2_ENABLE();
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }
    }  

    return ret_value ; 
}

static Std_ReturnType interrupt_INTx_clear_flag(const interrupt_INTx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        switch(int_obj->src)
        {
            case INTERRUPT_INT0:
                INT_INT0_CLEAR_FLAG();
                break;

            case INTERRUPT_INT1:
                INT_INT1_CLEAR_FLAG();
                break;

            case INTERRUPT_INT2:
                INT_INT2_CLEAR_FLAG();
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }

    }

    return ret_value ;
}

static Std_ReturnType INT0_set_callback(void ( *InterruptHandler) (void) )
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == InterruptHandler)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        INT0_InterruptHandler = InterruptHandler ;

    }

    return ret_value ;
}
static Std_ReturnType INT1_set_callback(void ( *InterruptHandler) (void) )
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == InterruptHandler)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        INT1_InterruptHandler = InterruptHandler ;

    }

    return ret_value ;
}
static Std_ReturnType INT2_set_callback(void ( *InterruptHandler) (void) )
{
    Std_ReturnType ret_value = E_OK ;
    if(NULL == InterruptHandler)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        INT2_InterruptHandler = InterruptHandler ;
    }
    
    return ret_value ;
}
static Std_ReturnType interrupt_INTx_set_callback(const interrupt_INTx_t *int_obj)
{

    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        switch(int_obj->src)
        {
            case INTERRUPT_INT0:
                ret_value = INT0_set_callback(int_obj->EXT_InterruptHandler);
                break;

            case INTERRUPT_INT1:
                ret_value = INT1_set_callback(int_obj->EXT_InterruptHandler);
                break;

            case INTERRUPT_INT2:
                ret_value = INT2_set_callback(int_obj->EXT_InterruptHandler);
                break;
            default:
                ret_value = E_NOT_OK;
                break;
        }

    }
    
    return ret_value ;
}




static Std_ReturnType interrupt_RBx_enable(const interrupt_RBx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        if( INT_HIGH_PRI == int_obj->priority)
        {
            INT_GLOBAL_HIGH_EN();
        }
        else if(INT_LOW_PRI == int_obj->priority)
        {
            INT_GLOBAL_LOW_EN();
            INT_GLOBAL_HIGH_EN();
        }
        else
        { /* Do Nothing */ }
        #else
        INT_GLOBAL_EN();
        INT_PERIPHERAL_EN();

        #endif
        
        INT_RBx_ENABLE();

    }

    return ret_value ;
}
static Std_ReturnType interrupt_RBx_disable(const interrupt_RBx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        #if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
        if( INT_HIGH_PRI == int_obj->priority)
        {
            INT_GLOBAL_HIGH_DIS();
        }
        else if(INT_LOW_PRI == int_obj->priority)
        {
            INT_GLOBAL_LOW_DIS();
        }
        else
        { /* Do Nothing */ }
        #else
        INT_GLOBAL_DIS();
        INT_PERIPHERAL_DIS();

        #endif
        
        INT_RBx_DISABLE();
    }

    return ret_value ;
    
}

static Std_ReturnType interrupt_RBx_pin_init(const interrupt_RBx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        gpio_pin_direction_init(&(int_obj->pin));
    }

    return ret_value ;

}

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
static Std_ReturnType interrupt_RBx_set_priority(const interrupt_RBx_t *int_obj)
{
    Std_ReturnType ret_value = E_OK ;

    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        INT_PRI_FET_EN();
        if(INT_HIGH_PRI == int_obj->priority)
        {
            INT_RBx_PRI_HIGH();
        }
        else if(INT_LOW_PRI == int_obj->priority)
        {
            INT_RBx_PRI_LOW();
        }
        else {}
    }

    return ret_value ;
    
}
#endif


static Std_ReturnType RB4_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) )
{
    
    Std_ReturnType ret_value = E_OK ;
    if((NULL == InterruptHandler_HIGH) && (NULL == InterruptHandler_LOW))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        RB4_InterruptHandler_HIGH = InterruptHandler_HIGH ;
        RB4_InterruptHandler_LOW  = InterruptHandler_LOW ;
    }
    return ret_value ;
}
static Std_ReturnType RB5_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) )
{
    
    Std_ReturnType ret_value = E_OK ;
    if((NULL == InterruptHandler_HIGH) && (NULL == InterruptHandler_LOW))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        RB5_InterruptHandler_HIGH = InterruptHandler_HIGH ;
        RB5_InterruptHandler_LOW  = InterruptHandler_LOW ;
    }
    return ret_value ;
}
static Std_ReturnType RB6_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) )
{
    
    Std_ReturnType ret_value = E_OK ;
    if((NULL == InterruptHandler_HIGH) && (NULL == InterruptHandler_LOW))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        RB6_InterruptHandler_HIGH = InterruptHandler_HIGH ;
        RB6_InterruptHandler_LOW  = InterruptHandler_LOW ;
    }
    return ret_value ;
}
static Std_ReturnType RB7_set_callback(void ( *InterruptHandler_HIGH) (void),
                                 void ( *InterruptHandler_LOW) (void) )
{
    
    Std_ReturnType ret_value = E_OK ;
    if((NULL == InterruptHandler_HIGH) && (NULL == InterruptHandler_LOW))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        RB7_InterruptHandler_HIGH = InterruptHandler_HIGH ;
        RB7_InterruptHandler_LOW  = InterruptHandler_LOW ;
    }
    return ret_value ;
}

static Std_ReturnType interrupt_RBx_set_callback(const interrupt_RBx_t *int_obj)
{
    
    Std_ReturnType ret_value = E_OK ;
    if(NULL == int_obj)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        switch(int_obj->pin.pin)
        {
            case PIN4:
                ret_value = RB4_set_callback(int_obj->EXT_InterruptHandler_HIGH ,int_obj->EXT_InterruptHandler_LOW );
                break;
                
            case PIN5:
                ret_value = RB5_set_callback(int_obj->EXT_InterruptHandler_HIGH ,int_obj->EXT_InterruptHandler_LOW );

                break;
            
            case PIN6:
                ret_value = RB6_set_callback(int_obj->EXT_InterruptHandler_HIGH ,int_obj->EXT_InterruptHandler_LOW );

                break;
            
            case PIN7:
                ret_value = RB7_set_callback(int_obj->EXT_InterruptHandler_HIGH ,int_obj->EXT_InterruptHandler_LOW );
                break;
            
            default:
                ret_value = E_NOT_OK ;
                break;
                     
        }
        
    }
    return ret_value ;
}