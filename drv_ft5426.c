/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-15     liuduanfei   first version
 */

#include <board.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#define DBG_TAG "ft5x26"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

struct rt_i2c_bus_device * ft54x6;
rt_uint8_t read_buf[4];

static void ft54x6_write_reg(rt_uint8_t reg, rt_uint8_t *value)
{
    struct rt_i2c_msg msg = {0};
    rt_uint8_t buf[2];
    buf[0] = reg;
    buf[1] = *value;

    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_WR;
    msg.buf = buf;
    msg.len = 2;
    rt_i2c_transfer(ft54x6, &msg, 1);
}

static void ft54x6_read_reg(rt_uint8_t reg, rt_uint8_t *value)
{
    struct rt_i2c_msg msg = {0};
    rt_uint8_t buf;
    buf = reg;

    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_WR;
    msg.buf = &buf;
    msg.len = 1;
    rt_i2c_transfer(ft54x6, &msg, 1);

    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_RD;
    msg.buf = value;
    msg.len = 1;
    rt_i2c_transfer(ft54x6, &msg, 1);
}

void ft54x6_get_point(rt_int16_t *x, rt_int16_t *y)
{
    struct rt_i2c_msg msg = {0};

    rt_uint8_t buf[4]  = {0};
    rt_uint8_t reg = 0x3;

    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_WR;
    msg.buf = &reg;
    msg.len = 1;
    rt_i2c_transfer(ft54x6, &msg, 1);

    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_RD;
    msg.buf = buf;
    msg.len = 4;
    rt_i2c_transfer(ft54x6, &msg, 1);

    *x = ((buf[0]&0X0F)<<8)+buf[1];
    *y = ((buf[2]&0X0F)<<8)+buf[3];
}

rt_bool_t ft54x6_get_status()
{
    rt_uint8_t value;

    ft54x6_read_reg(0x3, &value);

    if (((value>>6)&0x3) == 0x2)
    {
        return 1;
    }
    else if (((value>>6)&0x3) == 0x1)
    {
        return 0;
    }
    return 0;
}

int ft54x6_init(void)
{
    rt_uint8_t temp;

    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET);
    rt_thread_delay(50);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET);
    rt_thread_delay(50);

    ft54x6 = rt_i2c_bus_device_find("i2c0");

    if (ft54x6 == RT_NULL)
    {
        LOG_I("can not find i2c0");
        return -RT_EIO;
    }

    temp = 0;
    ft54x6_write_reg(0x0, &temp);
    temp = 0;
    ft54x6_write_reg(0xA4, &temp);
    temp = 22;
    ft54x6_write_reg(0x80, &temp);
    temp = 12;
    ft54x6_write_reg(0x88, &temp);

    return 0;
}
INIT_ENV_EXPORT(ft54x6_init);

int ft54x6_pin_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    //PH7
    GPIO_Initure.Pin=GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    //PI8
    GPIO_Initure.Pin=GPIO_PIN_8;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);

    return 0;
}
INIT_BOARD_EXPORT(ft54x6_pin_init);


#ifdef RT_USING_FINSH

void read_point(void *parameter)
{
    rt_int16_t x, y;
    rt_bool_t sta;
    while(1)
    {
        ft54x6_get_point(&x, &y);
        sta = ft54x6_get_status();
        rt_kprintf("%s:x:%d,y:%d\n", sta? "down":"up", x, y);

        rt_thread_delay(200);
    }
}

void ft54x6_test(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("ft54x6", read_point, RT_NULL, 512, 20, 20);
    if (tid == RT_NULL)
    {
        rt_kprintf("create ft54x6 thread err\n");
        return ;
    }
    rt_thread_startup(tid);
}
MSH_CMD_EXPORT(ft54x6_test, test touch...)

#endif
