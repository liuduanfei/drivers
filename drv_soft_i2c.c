#include <board.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

/*
 *
 *  I2C0
 *  SDA PI3
 *  SCL PH6
 *  slave: FT5426
 *
 *
 *  I2C2
 *  SDA PH5
 *  SCL PH4
 *  slave: PCF8574T, AT24C02, WM8978, MPU9250, AP3216C
 *
 */

/*
 * I2C0
 */
struct rt_i2c_bit_ops *i2c0_ops;
struct rt_i2c_bus_device *i2c0_bus;

void i2c0_set_sda(void *data, rt_int32_t state)
{
    // SDA PI3
    GPIOI->MODER &=~(0x3<<6);
    GPIOI->MODER |= (0x1<<6);
    if (state)
        GPIOI->BSRR = (0x1<<3);
    else
        GPIOI->BSRR = (0x1<<19);
}

void i2c0_set_scl(void *data, rt_int32_t state)
{
    // SCL PH6
    GPIOH->MODER &=~(0x3<<12);
    GPIOH->MODER |= (0x1<<12);
    if (state)
        GPIOH->BSRR = (0x1<<6);
    else
        GPIOH->BSRR = (0x1<<22);
}

rt_int32_t i2c0_get_sda(void *data)
{
    // SDA PI3
    GPIOI->MODER &=~(0x3<<6);
    return (GPIOI->IDR&(0x1<<3));
}
rt_int32_t i2c0_get_scl(void *data)
{
    // SCL PH6
    GPIOH->MODER &=~(0x3<<12);
    return (GPIOH->IDR&(0x1<<6));
}

int i2c0_pin_init()
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;
    GPIO_Initure.Pull=GPIO_NOPULL;
    GPIO_Initure.Pin=GPIO_PIN_6;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_3;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
    return 0;
}
INIT_BOARD_EXPORT(i2c0_pin_init);

int i2c0_register(void)
{
    i2c0_ops = rt_malloc(sizeof(struct rt_i2c_bit_ops));
    i2c0_ops->set_sda = i2c0_set_sda;
    i2c0_ops->set_scl = i2c0_set_scl;
    i2c0_ops->get_sda = i2c0_get_sda;
    i2c0_ops->get_scl = i2c0_get_scl;
    i2c0_ops->data = RT_NULL;
    i2c0_ops->delay_us = 0;
    i2c0_ops->udelay = rt_hw_us_delay;
    i2c0_ops->timeout = 100;

    i2c0_bus = rt_malloc(sizeof(struct rt_i2c_bus_device));
    i2c0_bus->priv = i2c0_ops;
    i2c0_bus->retries = 0;

    rt_i2c_bit_add_bus(i2c0_bus, "i2c0");
    return 0;
}
INIT_BOARD_EXPORT(i2c0_register);

struct rt_i2c_bus_device * i2c0;


/*
 * I2C2
 */
struct rt_i2c_bit_ops *i2c2_ops;
struct rt_i2c_bus_device *i2c2_bus;

void i2c2_set_sda(void *data, rt_int32_t state)
{
    GPIOH->MODER &=~(0x3<<10);
    GPIOH->MODER |= (0x1<<10);
    if (state)
        GPIOH->BSRR = (0x1<<5);
    else
        GPIOH->BSRR = (0x1<<21);
}

void i2c2_set_scl(void *data, rt_int32_t state)
{
    GPIOH->MODER &=~(0x3<<8);
    GPIOH->MODER |= (0x1<<8);
    if (state)
        GPIOH->BSRR = (0x1<<4);
    else
        GPIOH->BSRR = (0x1<<20);
}
rt_int32_t i2c2_get_sda(void *data)
{
    GPIOH->MODER &=~(0x3<<10);
    return (GPIOH->IDR&(0x1<<5));
}
rt_int32_t i2c2_get_scl(void *data)
{
    GPIOH->MODER &=~(0x3<<8);
    return (GPIOH->IDR&(0x1<<4));
}

int i2c2_pin_init()
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Initure.Pin=GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;
    GPIO_Initure.Pull=GPIO_NOPULL;
    GPIO_Initure.Pin=GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_SET);

    return 0;
}
INIT_BOARD_EXPORT(i2c2_pin_init);

int i2c2_register(void)
{
    i2c2_ops = rt_malloc(sizeof(struct rt_i2c_bit_ops));
    i2c2_ops->set_sda = i2c2_set_sda;
    i2c2_ops->set_scl = i2c2_set_scl;
    i2c2_ops->get_sda = i2c2_get_sda;
    i2c2_ops->get_scl = i2c2_get_scl;
    i2c2_ops->data = RT_NULL;
    i2c2_ops->delay_us = 0;
    i2c2_ops->udelay = rt_hw_us_delay;
    i2c2_ops->timeout = 100;

    i2c2_bus = rt_malloc(sizeof(struct rt_i2c_bus_device));
    i2c2_bus->priv = i2c2_ops;
    i2c2_bus->retries = 0;

    rt_i2c_bit_add_bus(i2c2_bus, "i2c2");
    return 0;
}
INIT_BOARD_EXPORT(i2c2_register);



