#include "mWs2812.h"
#include"eSpi1Driver.h"

#define WS2812_FILL_BUFFER(COLOR) \
    for( uint8_t mask = 0x80; mask; mask >>= 1 ) { \
        if( COLOR & mask ) { *ptr++ = 0xfc; } \
        else { *ptr++ = 0x80; }}

uint8_t ws2812_buffer[WS2812_BUFFER_SIZE];

  uint8_t add = 0, r = 0, g = 0, b = 0;


void ws2812_init(void) {
    memset(ws2812_buffer, 0, WS2812_BUFFER_SIZE);
    ws2812_send_spi();
}

void ws2812_send_spi(void)
{
eSpiDmaSend(ws2812_buffer, sizeof(ws2812_buffer));
}

void ws2812_pixel(uint16_t led_no, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t * ptr = &ws2812_buffer[24 * led_no];
    WS2812_FILL_BUFFER(g);
    WS2812_FILL_BUFFER(r);
    WS2812_FILL_BUFFER(b);
}

void ws2812_pixel_all(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t * ptr = ws2812_buffer;
    for( uint16_t i = 0; i < WS2812_NUM_LEDS; ++i) {
        WS2812_FILL_BUFFER(g);
        WS2812_FILL_BUFFER(r);
        WS2812_FILL_BUFFER(b);
    }
}


void  ws2812ShowOnePoint(uint8_t  pwm)
{
uint8_t i;
   add = (add+ 1) % WS2812_NUM_LEDS;

ws2812_pixel_all(0,0,0);
		{
		ws2812_pixel(add,pwm,pwm,pwm);
		}
    ws2812_send_spi();

}


void  mWS2812RunApp(void)
{
   add = (add+ 1) % WS2812_NUM_LEDS;
	ws2812ShowOnePoint(add);
//    ws2812_pixel(i, r++, g++, b++);
//	ws2812_pixel_all(r++, g++, b++);
    ws2812_send_spi();
}
