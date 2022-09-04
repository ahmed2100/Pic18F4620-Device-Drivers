/* 
 * File:   app.c
 * Author: ahmed
 *
 * Created on July 11, 2022, 10:31 PM
 */

/* Led Blink Example */

#include "app.h"
#include "ECU_Layer/LED/ecu_led.h"



led_t led1 = 
{
    .port_name = PORTC_INDEX ,
    .pin = PIN0 ,
    .led_status = LED_OFF 
};

int main() {
    app_init();
    while(1)
    {
        ecu_led_turn_toggle(&led1);
        __delay_ms(1000);

    }
    return (EXIT_SUCCESS);
}
/**
 * @brief initializes modules used in application 
 */

void app_init(void) 
{    
    ecu_led_init(&led1);
}

