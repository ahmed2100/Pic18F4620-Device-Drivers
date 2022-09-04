/* 
 * File:   ecu_chr_lcd.h
 * Author: ahmed
 *
 * Created on July 30, 2022, 8:42 AM
 */

#ifndef ECU_CHR_LCD_H
#define	ECU_CHR_LCD_H


/* Includes */
#include "ecu_chr_lcd_config.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/*---------------------------------------- Macro Defines ---------------------------------------- */
/* clear display */
#define _LCD_CLEAR                          0x01
/* cursor home */
#define _LCD_RETURN_HOME                    0x02
/* entry mode set */                    
#define _LCD_CURSOR_INC_SHIFT_OFF           0x06
#define _LCD_CURSOR_DEC_SHIFT_OFF           0x04
#define _LCD_ENT_SHIFT_LEFT                 0x07
#define _LCD_ENT_SHIFT_RIGHT                0x05
/* display off/on */
#define _LCD_OFF                            0x08
#define _LCD_CURSOR_OFF_BLINK_OFF           0x0C
#define _LCD_CURSOR_OFF_BLINK_ON            0x0D
#define _LCD_CURSOR_ON_BLINK_OFF            0x0E
#define _LCD_CURSOR_ON_BLINK_ON             0x0F
/* cursor or display shift */
#define _LCD_DISPLAY_SHIFT_LEFT             0X18
#define _LCD_DISPLAY_SHIFT_RIGHT            0X1C

#define LCD_16_X_2_Starting_ADD_ROW_1       0x80
#define LCD_16_X_2_Starting_ADD_ROW_2       0xC0

#define LCD_20_X_4_Starting_ADD_ROW_1       0x80
#define LCD_20_X_4_Starting_ADD_ROW_2       0xC0
#define LCD_20_X_4_Starting_ADD_ROW_3       0x94
#define LCD_20_X_4_Starting_ADD_ROW_4       0xD4


/* function set */
#define _LCD_8bit_MODE_2_LINE               0x38
#define _LCD_4bit_MODE_2_LINE               0x28
/* address set */
#define _LCD_DDRAM_START                    0x80
#define _LCD_CGRAM_START                    0x40

#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4


/* Macro Functions */

/* User-Defined Data Type Declaration */

typedef struct {
    pin_config_t lcd_rs ;
    pin_config_t lcd_en ;
    pin_config_t lcd_data[4] ;

}chr_lcd_4bit_t;

typedef struct {
    pin_config_t lcd_rs ;
    pin_config_t lcd_en ;
    pin_config_t lcd_data[8] ;
}chr_lcd_8bit_t;


/* Software Interfaces (Prototypes) */
Std_ReturnType ecu_4bit_chr_lcd_init(const chr_lcd_4bit_t *lcd);
Std_ReturnType ecu_4bit_chr_lcd_send_command(const chr_lcd_4bit_t *lcd , uint8_t command);
Std_ReturnType ecu_4bit_chr_lcd_send_char_data(const chr_lcd_4bit_t *lcd , uint8_t character);
Std_ReturnType ecu_4bit_chr_lcd_send_char_data_pos(const chr_lcd_4bit_t *lcd , 
                                                        uint8_t character , 
                                                        uint8_t row , uint8_t col);

Std_ReturnType ecu_4bit_chr_lcd_send_str_data(const chr_lcd_4bit_t *lcd , uint8_t *str);
Std_ReturnType ecu_4bit_chr_lcd_send_str_data_pos(const chr_lcd_4bit_t *lcd , uint8_t *str , 
                                                    uint8_t row , uint8_t col);
Std_ReturnType ecu_4bit_chr_lcd_send_custom_char(const chr_lcd_4bit_t *lcd , uint8_t c_chr[] , uint8_t mem_pos , 
                                                uint8_t row , uint8_t col);





Std_ReturnType ecu_8bit_chr_lcd_init(const chr_lcd_8bit_t *lcd);
Std_ReturnType ecu_8bit_chr_lcd_send_command(const chr_lcd_8bit_t *lcd , uint8_t command);
Std_ReturnType ecu_8bit_chr_lcd_send_char_data(const chr_lcd_8bit_t *lcd , uint8_t character);
Std_ReturnType ecu_8bit_chr_lcd_send_char_data_pos(const chr_lcd_8bit_t *lcd , 
                                                        uint8_t character , 
                                                        uint8_t row , uint8_t col);

Std_ReturnType ecu_8bit_chr_lcd_send_str_data(const chr_lcd_8bit_t *lcd , uint8_t *str);
Std_ReturnType ecu_8bit_chr_lcd_send_str_data_pos(const chr_lcd_8bit_t *lcd , uint8_t *str , 
                                                    uint8_t row , uint8_t col);
Std_ReturnType ecu_8bit_chr_lcd_send_custom_char(const chr_lcd_8bit_t *lcd , uint8_t c_chr[] , uint8_t mem_pos , 
                                                uint8_t row , uint8_t col);


Std_ReturnType ecu_uint8_to_str(uint8_t value , uint8_t *str);
Std_ReturnType ecu_uint16_to_str(uint16_t value , uint8_t *str);
Std_ReturnType ecu_uint32_to_str(uint32_t value , uint8_t *str);
Std_ReturnType ecu_int32_to_str(int32_t value , uint8_t *str);



/* Global Shared Variables */

#endif	/* ECU_CHR_LCD_H */

