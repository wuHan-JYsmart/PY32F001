#ifndef __M_WS2812_APP_H
#define __M_WS2812_APP_H
#include"eTypedef.h"

#define WS2812_NUM_LEDS 12


#define WS2812_RESET_PULSE 60
#define WS2812_BUFFER_SIZE (WS2812_NUM_LEDS * 24 + WS2812_RESET_PULSE)


extern uint8_t ws2812_buffer[];

void ws2812_init(void);
void ws2812_send_spi(void);
void ws2812_pixel(uint16_t led_no, uint8_t r, uint8_t g, uint8_t b);
void ws2812_pixel_all(uint8_t r, uint8_t g, uint8_t b);

void  mWS2812RunApp(void);
void  ws2812ShowOnePoint(uint8_t  pwm);


#endif

