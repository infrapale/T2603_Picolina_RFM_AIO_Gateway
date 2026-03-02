#include <Arduino.h>
#include "atask.h"
#include "main.h"
#include "io.h"

typedef struct
{
  uint8_t pin;
  uint32_t pattern;
} led_st;

typedef struct
{
  uint8_t pattern_bit;
  uint8_t switches;
  uint8_t tindx;
} io_ctrl_st;

io_ctrl_st io_ctrl;

led_st led[COLOR_NBR_OF] =
{
    {PIN_LED_RED, 0},
    {PIN_LED_YELLOW, 0},
    {PIN_LED_BLUE, 0},
};

const uint32_t led_pattern[BLINK_NBR_OF] = 
{
    0b0000000000000000,
    0b1111111111111111,
    0b1000000000000000,
    0b1000000100000000,
    0b1001001001001000,
    0b1111111100000000,
    0b1111000011110000,
    0b1100110011001100,
    0b1111000000100000,
    0b1111000010001000,
    0b1111001010101010,
    0b1111001100110000,


};

void io_task(void);
//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st io_task_handle       =   {"I/O Task       ", 100,     0,     0,  255,    0,  1,  io_task };

void io_initialize(void)
{
  analogReadResolution(12);
  //RFM95 Reset
  pinMode(PIN_RFM_RESET, OUTPUT);
  digitalWrite(PIN_RFM_RESET, HIGH);

  io_ctrl.pattern_bit = 0;
  for (uint8_t i = COLOR_RED; i <= COLOR_BLUE; i++)
  {
    pinMode(led[i].pin, OUTPUT);
    digitalWrite(led[i].pin, LOW);
  } 
}
void io_task_initialize(void)
{
    io_ctrl.tindx =  atask_add_new(&io_task_handle);
}


void io_led_flash(color_et color, blink_et bindx)
{
  led[color].pattern = led_pattern[bindx];
}

void io_task(void)
{

  uint32_t patt = 1UL << io_ctrl.pattern_bit;
  for (uint8_t i = COLOR_RED; i <= COLOR_BLUE; i++)
  {
    if ((led[i].pattern & patt) != 0)
        digitalWrite(led[i].pin, HIGH);
    else  
        digitalWrite(led[i].pin, LOW);
  } 
  if (++io_ctrl.pattern_bit >= 16) io_ctrl.pattern_bit = 0;
}