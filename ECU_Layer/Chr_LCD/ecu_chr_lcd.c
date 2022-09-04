/* 
 * File:   ecu_chr_lcd.c
 * Author: ahmed
 *
 * Created on July 30, 2022, 8:42 AM
 */

#include "ecu_chr_lcd.h"

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd , uint8_t  _data_command);
static Std_ReturnType lcd_4bit_send_enable(const chr_lcd_4bit_t *lcd);
static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8_t row ,uint8_t col );

static Std_ReturnType lcd_send_8bits(const chr_lcd_8bit_t *lcd , uint8_t  _data_command);
static Std_ReturnType lcd_8bit_send_enable(const chr_lcd_8bit_t *lcd);
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8_t row ,uint8_t col ); 

/* Implementation */

Std_ReturnType ecu_4bit_chr_lcd_init(const chr_lcd_4bit_t *lcd)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_counter = 0 ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // init all pins
        ret_value =  gpio_pin_init(&(lcd->lcd_rs));
        ret_value &= gpio_pin_init(&(lcd->lcd_en));

        for(l_counter = 0 ; l_counter < 4  ; l_counter++)
        {
            ret_value &= gpio_pin_init( &(lcd->lcd_data[l_counter]) );
        }

         __delay_ms(40);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        __delay_ms(5);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        __delay_us(150);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        
        
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_4bit_MODE_2_LINE);
        ret_value &= ecu_4bit_chr_lcd_send_command(lcd , _LCD_CLEAR);
        ret_value &= ecu_4bit_chr_lcd_send_command(lcd , _LCD_RETURN_HOME);
        ret_value &= ecu_4bit_chr_lcd_send_command(lcd , _LCD_CURSOR_OFF_BLINK_OFF);
        ret_value &= ecu_4bit_chr_lcd_send_command(lcd , _LCD_4bit_MODE_2_LINE);
        ret_value &= ecu_4bit_chr_lcd_send_command(lcd , _LCD_DDRAM_START);
    }

    return ret_value ;
}
Std_ReturnType ecu_4bit_chr_lcd_send_command(const chr_lcd_4bit_t *lcd , uint8_t command)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // put logic 0 on R/W (hardwired)

        ret_value &= gpio_pin_write_logic(&(lcd->lcd_rs) ,GPIO_LOW );
        ret_value &= lcd_send_4bits(lcd ,(command >> 4));
        ret_value &= lcd_4bit_send_enable(lcd);
        ret_value &= lcd_send_4bits(lcd ,(command));
        ret_value &= lcd_4bit_send_enable(lcd);


    }

    return ret_value ;
    
}
Std_ReturnType ecu_4bit_chr_lcd_send_char_data(const chr_lcd_4bit_t *lcd , uint8_t character)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // put logic 0 on R/W (hardwired)

        ret_value &= gpio_pin_write_logic(&(lcd->lcd_rs) ,GPIO_HIGH );
        ret_value &= lcd_send_4bits(lcd ,(character >> 4));
        ret_value &= lcd_4bit_send_enable(lcd);
        __delay_us(5);
        ret_value &= lcd_send_4bits(lcd ,(character));
        ret_value &= lcd_4bit_send_enable(lcd);

    }

    return ret_value ;
    
}
Std_ReturnType ecu_4bit_chr_lcd_send_char_data_pos(const chr_lcd_4bit_t *lcd , 
                                                        uint8_t character , 
                                                        uint8_t row , uint8_t col)
{
    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        lcd_4bit_set_cursor(lcd , row , col);
        ecu_4bit_chr_lcd_send_char_data(lcd , character);
    }

    return ret_value ;
    
}

Std_ReturnType ecu_4bit_chr_lcd_send_str_data(const chr_lcd_4bit_t *lcd , uint8_t *str)
{
    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == lcd) ||  (NULL == str) )
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        while(*str)
        {
            ecu_4bit_chr_lcd_send_char_data(lcd , *str);
            str++;
        }

    }

    return ret_value ;
    
}
Std_ReturnType ecu_4bit_chr_lcd_send_str_data_pos(const chr_lcd_4bit_t *lcd , uint8_t *str , 
                                                    uint8_t row , uint8_t col)
{

    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == lcd) ||  (NULL == str) )
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ret_value =  lcd_4bit_set_cursor(lcd , row , col);
        ret_value &= ecu_4bit_chr_lcd_send_str_data(lcd , str);

    }

    return ret_value ;
    
}
Std_ReturnType ecu_4bit_chr_lcd_send_custom_char(const chr_lcd_4bit_t *lcd , uint8_t c_chr[] , uint8_t mem_pos , 
                                                uint8_t row , uint8_t col)
{

    Std_ReturnType ret_value = E_OK ;
    uint8_t l_lcd_counter = 0 ;

    mem_pos-- ;
    if((NULL == lcd) ||  (NULL == c_chr) )
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ret_value = ecu_4bit_chr_lcd_send_command(lcd , (_LCD_CGRAM_START + (mem_pos * 8) ));
        for(l_lcd_counter = 0 ; l_lcd_counter < 8 ; l_lcd_counter++)
        {
            ret_value &= ecu_4bit_chr_lcd_send_char_data(lcd , c_chr[l_lcd_counter]);
        }
        ret_value &= ecu_4bit_chr_lcd_send_char_data_pos(lcd , mem_pos , row , col);

    }

    return ret_value ;
    
}





Std_ReturnType ecu_8bit_chr_lcd_init(const chr_lcd_8bit_t *lcd)
{

    Std_ReturnType ret_value = E_OK ;
    uint8_t l_counter = 0 ;

    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // init all pins
        ret_value =  gpio_pin_init(&(lcd->lcd_rs));
        ret_value &= gpio_pin_init(&(lcd->lcd_en));

        for(l_counter = 0 ; l_counter < 8  ;l_counter++)
        {
            ret_value &= gpio_pin_init(&(lcd->lcd_data[l_counter]));
        }
        __delay_ms(20);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        __delay_ms(5);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        __delay_us(150);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_CLEAR);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_RETURN_HOME);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_CURSOR_INC_SHIFT_OFF);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_CURSOR_OFF_BLINK_OFF);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_8bit_MODE_2_LINE);
        ret_value &= ecu_8bit_chr_lcd_send_command(lcd , _LCD_DDRAM_START);
        

    }

    return ret_value ;
    
}
Std_ReturnType ecu_8bit_chr_lcd_send_command(const chr_lcd_8bit_t *lcd , uint8_t command)
{

    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // put logic 0 on R/W (hardwired)
        ret_value &= gpio_pin_write_logic(&(lcd->lcd_rs) ,GPIO_LOW );
        ret_value &= lcd_send_8bits(lcd ,command );
        ret_value &= lcd_8bit_send_enable(lcd);
    }

    return ret_value ;
    
}
Std_ReturnType ecu_8bit_chr_lcd_send_char_data(const chr_lcd_8bit_t *lcd , uint8_t character)
{

    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        // put logic 0 on R/W (hardwired)

        ret_value &= gpio_pin_write_logic(&(lcd->lcd_rs) ,GPIO_HIGH );
        ret_value &= lcd_send_8bits(lcd ,character);
        ret_value &= lcd_8bit_send_enable(lcd);

    }

    return ret_value ;
    
}
Std_ReturnType ecu_8bit_chr_lcd_send_char_data_pos(const chr_lcd_8bit_t *lcd , 
                                                        uint8_t character , 
                                                        uint8_t row , uint8_t col)
{

    Std_ReturnType ret_value = E_OK ;
    
    if(NULL == lcd)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ret_value = lcd_8bit_set_cursor(lcd , row , col);
        ret_value &= ecu_8bit_chr_lcd_send_char_data(lcd , character);

    }

    return ret_value ;
    
}

Std_ReturnType ecu_8bit_chr_lcd_send_str_data(const chr_lcd_8bit_t *lcd , uint8_t *str)
{

    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == lcd) || (NULL == str))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        
        while(*str)
        {
            ret_value &= ecu_8bit_chr_lcd_send_char_data(lcd , *str);
            str++;
        }

    }

    return ret_value ;
    
}
Std_ReturnType ecu_8bit_chr_lcd_send_str_data_pos(const chr_lcd_8bit_t *lcd , uint8_t *str , 
                                                    uint8_t row , uint8_t col)
{
    Std_ReturnType ret_value = E_OK ;
    
    if((NULL == lcd )|| (NULL == str))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ret_value =  lcd_8bit_set_cursor(lcd , row , col);
        ret_value &= ecu_8bit_chr_lcd_send_str_data(lcd , str);

    }

    return ret_value ;
    
}
Std_ReturnType ecu_8bit_chr_lcd_send_custom_char(const chr_lcd_8bit_t *lcd , uint8_t c_chr[] , uint8_t mem_pos , 
                                                uint8_t row , uint8_t col)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_lcd_counter = 0 ;

    --mem_pos ;
    if((NULL == lcd )|| (NULL == c_chr))
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        ret_value = ecu_8bit_chr_lcd_send_command(lcd , (_LCD_CGRAM_START + (mem_pos * 8) ));
        for(l_lcd_counter = 0 ; l_lcd_counter < 8 ; l_lcd_counter++)
        {
            ret_value &= ecu_8bit_chr_lcd_send_char_data(lcd , c_chr[l_lcd_counter]);
        }
        ret_value &= ecu_8bit_chr_lcd_send_char_data_pos(lcd , mem_pos , row , col);
    }

    return ret_value ;
    
    
}


Std_ReturnType ecu_uint8_to_str(uint8_t value , uint8_t *str)
{
     Std_ReturnType ret_value = E_OK ;
    
    if(NULL == str)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        memset(str , '\0' , 4);
        sprintf(str , "%i" , value);
    }

    return ret_value ;
    
    
}
Std_ReturnType ecu_uint16_to_str(uint16_t value , uint8_t *str)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t l_counter = 0 ;
    
    if(NULL == str)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {
        *(str + 6) = '\0';
        memset(str , ' ' , 5);
        sprintf(str , "%i" , value);
        for(l_counter = 0 ; l_counter < 6 ; l_counter++)
        {
            if('\0' ==str[l_counter])
            {
                str[l_counter] = ' ';
            }
            else
            {}
        }
    }

    return ret_value ;
}
Std_ReturnType ecu_uint32_to_str(uint32_t value , uint8_t *str)
{
     Std_ReturnType ret_value = E_OK ;
    
    if(NULL == str)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {

        memset(str , '\0' , 11);
        sprintf(str , "%lu" , value);

    }

    return ret_value ;
}

Std_ReturnType ecu_int32_to_str(int32_t value , uint8_t *str)
{
     Std_ReturnType ret_value = E_OK ;
    
    if(NULL == str)
    {
        ret_value = E_NOT_OK ;
    }
    else
    {            
        memset(str , '\0' , 11);

        if(value < 0)
        {
            *str = '-';
            str++ ;
        }
            sprintf(str , "%i" , value);

    }

    return ret_value ;
}

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd , uint8_t  _data_command)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t counter = 0 ;

    for(counter = 0 ; counter < 4 ; counter++)
    {
        ret_value &= gpio_pin_write_logic( &(lcd->lcd_data[counter])  ,
                                             ( (_data_command >> counter) & (uint8_t) 0x01 ) );
    }

    return ret_value;

}

static Std_ReturnType lcd_send_8bits(const chr_lcd_8bit_t *lcd , uint8_t  _data_command)
{
    Std_ReturnType ret_value = E_OK ;
    uint8_t counter = 0 ;

    for(counter = 0 ; counter < 8 ; counter++)
    {
        ret_value &= gpio_pin_write_logic( &(lcd->lcd_data[counter])  ,
                                             ( (_data_command >> counter) & (uint8_t) 0x01 ) );
    }

    return ret_value;

}


static Std_ReturnType lcd_4bit_send_enable(const chr_lcd_4bit_t *lcd)
{
    Std_ReturnType ret_value = E_OK ;

    ret_value = gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_HIGH);
    __delay_us(5);
    ret_value &= gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_LOW);


    return ret_value;
}

static Std_ReturnType lcd_8bit_send_enable(const chr_lcd_8bit_t *lcd)
{
    Std_ReturnType ret_value = E_OK ;

    ret_value = gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_HIGH);
    __delay_us(5);
    ret_value &= gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_LOW);


    return ret_value;

}

static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8_t row ,uint8_t col )
{
    Std_ReturnType ret_value = E_OK ;
    if((NULL == lcd) || (row <= 0) || (col <= 0))
    {
        ret_value = E_NOT_OK ;
    }

    else
    {
        --col ;
        switch (row)
        {
        case ROW1:
            ret_value = ecu_8bit_chr_lcd_send_command(lcd , (LCD_20_X_4_Starting_ADD_ROW_1 + col));
            break;

        case ROW2:
            ret_value = ecu_8bit_chr_lcd_send_command(lcd , (LCD_20_X_4_Starting_ADD_ROW_2 + col));

            break;

        case ROW3:
            ret_value = ecu_8bit_chr_lcd_send_command(lcd , (LCD_20_X_4_Starting_ADD_ROW_3 + col));

            break;

        case ROW4:
            ret_value = ecu_8bit_chr_lcd_send_command(lcd , (LCD_20_X_4_Starting_ADD_ROW_4 + col));

            break;
        default:
            ret_value = E_NOT_OK;

            break;
        }

    }
    
    return ret_value ;
}

static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8_t row ,uint8_t col )
{
    Std_ReturnType ret_value = E_OK ;
    if((NULL == lcd) || (row <= 0) || (col <= 0))
    {
        ret_value = E_NOT_OK ;
    }

    else
    {
        --col ;
        switch (row)
        {
        case ROW1:
            ecu_4bit_chr_lcd_send_command(lcd , (LCD_16_X_2_Starting_ADD_ROW_1 + col));
            break;

        case ROW2:
            ecu_4bit_chr_lcd_send_command(lcd , (LCD_16_X_2_Starting_ADD_ROW_2 + col));

            break;

        case ROW3:
            ecu_4bit_chr_lcd_send_command(lcd , (LCD_20_X_4_Starting_ADD_ROW_3 + col));

            break;

        case ROW4:
            ecu_4bit_chr_lcd_send_command(lcd , (LCD_20_X_4_Starting_ADD_ROW_4  + col));

            break;
        default:
            ret_value = E_NOT_OK;

            break;
        }

    }

    return ret_value;
}
