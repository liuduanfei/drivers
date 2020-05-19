#include <board.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "drv_ft5426.h"

struct rt_i2c_bus_device * ft54x6;
rt_uint8_t read_buf[4];


static void ft54x6_write_reg(rt_uint8_t reg, rt_uint8_t *value)
{
    struct rt_i2c_msg msg;
    rt_uint8_t buf[2];
    buf[0] = reg;
    buf[1] = *value;

    rt_memcpy(&msg, 0, sizeof( struct rt_i2c_msg));
    msg.addr = (0x70>>1);
    msg.flags = RT_I2C_WR;
    msg.buf = buf;
    msg.len = 2;
    rt_i2c_transfer(ft54x6, &msg, 1);
}

static void ft54x6_read_reg(rt_uint8_t reg, rt_uint8_t *value)
{
    struct rt_i2c_msg msg;
    rt_uint8_t buf;
    buf = reg;

    rt_memcpy(&msg, 0, sizeof( struct rt_i2c_msg));
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
    struct rt_i2c_msg msg;
    rt_uint8_t buf[4];
    rt_memcpy(&msg, 0, sizeof( struct rt_i2c_msg));
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
//INIT_ENV_EXPORT(ft54x6_init);

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
//INIT_BOARD_EXPORT(ft54x6_pin_init);




