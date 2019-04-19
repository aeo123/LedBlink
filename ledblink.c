/*
 * @Description: 若offtime=0，则位设置单次闪烁
 * @Author: zpw
 * @LastEditors: zpw
 * @Date: 2019-04-19 18:30:33
 * @LastEditTime: 2019-04-19 19:15:01
 */

#include "drv_led.h"

static uint8_t led_num = 0;
static led_blink_struct *ls = 0;

/**
 * @brief  添加一个led设备
 * @note   
 * @param  led_pin: 引脚
 * @retval 
 */
int led_add_device(rt_base_t led_pin)
{
    //为新led分配内存
    ls = (led_blink_struct *)rt_malloc(ls, (led_num + 1) * sizeof(led_blink_struct));

    if (ls)
    {
        led_num++;
        ls->pin = led_pin;
        ls->ison = 0;
        ls->state = OFF;
        //init pin
        rt_pin_mode(led_pin, PIN_MODE_OUTPUT);
        return 1;
    }
    return 0;
}

/**
 * @brief  设置led引脚电平
 * @note   
 * @param  num: led编号，从1开始计数
 * @param  level: 引脚电平
 * @retval None
 */
void led_set_level(uint8_t num, uint8_t level)
{
    if (num > led_num || num < 1)
    {
        return;
    }
    rt_pin_write(ls[num - 1]->pin, level);
}

/**
 * @brief  点亮led
 * @note   
 * @param  num: 
 * @retval None
 */
void led_on(uint8_t num)
{
    if (num > led_num || num < 1)
    {
        return;
    }
    //set led state
    ls[num - 1]->state = ON;
    rt_pin_write(ls[num - 1]->pin, LED_ON_STATE);
}

/**
 * @brief  关闭led
 * @note   
 * @param  num: 
 * @retval None
 */
void led_off(uint8_t num)
{
    if (num > led_num || num < 1)
    {
        return;
    }
    //set led state
    ls[num - 1]->state = OFF;
    rt_pin_write(ls[num - 1]->pin, !LED_ON_STATE);
}

/**
 * @brief  翻转一次led电平
 * @note   
 * @param  num: 
 * @retval None
 */
void led_tog(uint8_t num)
{
    if (num > led_num || num < 1)
    {
        return;
    }
    //set led state
    ls[num - 1]->state = TOG;

    if (ls[num - 1]->ison)
    {
        //next off
        rt_pin_write(ls[num - 1]->pin, !LED_ON_STATE);
    }
    else
    {
        //next on
        rt_pin_write(ls[num - 1]->pin, LED_ON_STATE);
    }

    //tog status
    ls[num - 1]->ison = 1 - ls[num - 1]->ison;
}

/**
 * @brief  设置led为闪烁状态
 * @note   
 * @param  num: led编号，从1开始
 * @param  ontime: 一周期内led亮的时间，单位tick
 * @param  offtime: 一周期内led灭的时间，单位tick
 * @retval None
 */
void led_blink(uint8_t num, int ontime, int offtime)
{
    if (num > led_num || num < 1)
    {
        return;
    }
    //init status is on
    led_set_level(num, LED_ON_STATE);

    ls[num - 1]->ison = 1;
    ls[num - 1]->state = BLINK;
    ls[num - 1]->tick = ontime;
    ls[num - 1]->on_time = ontime;
    ls[num - 1]->off_time = offtime;
}

/**
 * @brief  led闪烁进程，传入时间增量
 * @note   
 * @param  inc: 传入函数运行的时间间隔，单位tick
 * @retval None
 */
void led_process(int inc)
{

    for (int i = 0; i < led_num; i++)
    {
        if (ls[i]->state == BLINK)
        {
            ls[i]->tick -= inc;
            if (ls[i]->ison && ls[i]->tick > 0)
            {
                //in ON STATE , do nothing
            }
            else if (ls[i]->ison && ls[i]->off_time == 0)
            {
                //offtime=0; just blink once
                ls[i]->state = OFF;
                led_on(i + 1);
            }
            else if (ls[i]->ison && ls[i]->off_time > 0)
            {
                //ison = 0; offtime>0;  On state turn to off state
                ls[i]->tick = ls[i]->off_time;
                ls[i]->ison = 0;
                led_off(i + 1);
            }
            else if (ls[i]->ison == 0 && ls[i]->tick > 0)
            {
                //OFF STATE , do nothing
            }
            else if (ls[i]->ison == 0)
            {
                // off state turn to ON state
                ls[i]->tick = ls[i]->on_time;
                ls[i]->ison = 1;
                led_on(i + 1);
            }
            else
            {
                ls[i]->ison = 0;
                led_off(i + 1);
            }
        }
    }
}
