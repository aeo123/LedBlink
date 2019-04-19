/*
 * @Description: 
 * @Author: zpw
 * @LastEditors: zpw
 * @Date: 2019-04-19 18:30:37
 * @LastEditTime: 2019-04-19 19:16:12
 */
#ifndef _BLINK_LED_H_
#define _BLINK_LED_H_

#include "string.h"
// #include "stdlib.h"
#include "drv_gpio.h"

#define LED_ON_STATE 1 //点亮的电平

typedef enum
{
    ON = 0,
    OFF,
    BLINK, //闪烁
    TOG,   //翻转一次
} led_status;

typedef struct
{
    rt_base_t pin;     //led引脚
    led_status state;  //模式
    uint8_t ison;      //当前状态
    uint32_t on_time;  //每周期亮的时间，单位tick
    uint32_t off_time; //每周期灭的时间，单位tick
    uint32_t tick;     //tick计数值
} led_blink_struct;

int led_add_device(rt_base_t led_pin);
void led_set_level(uint8_t num, uint8_t level);
void led_on(uint8_t num);
void led_off(uint8_t num);
void led_tog(uint8_t num);
void led_blink(uint8_t num, int ontime, int offtime);
void led_process(int inc);
#endif
