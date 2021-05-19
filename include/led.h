/*
 * led.h
 */

#ifndef MSP432_LED_H_
#define MSP432_LED_H_

#include <stdint.h>

#define LED1        0U
#define RGB_RED     0U
#define RGB_GREEN   1U
#define RGB_BLUE    2U


class Led{

public:

    Led();

    void rgb_led_on(uint8_t led_no);

    void rgb_led_off(uint8_t led_no);

    void rgb_led_toggle(uint8_t led_no);

    void led1_on(void);

    void led1_off(void);

    void led1_toggle(void); 

    ~Led() = default;

};

#endif /* LED_H_ */
