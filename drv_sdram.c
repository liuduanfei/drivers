/*
 * W9825G6KH-6 166MHz/CL3 133MHz/CL2
 *
 *        rcc_hclk3 *
 * clock  pll1_q_ck            fmc_ker_ck
 *        pll2_r_ck
 *        per_ck
 *
 * 8K Refresh Cycles/64ms
 * row 13 column 9 bank 4
 * 4M words x 4banks x 16bits
 * 100MHz == f10ns
 *
 * SDRAM datasheet      stm32h7 Reference manual
 *
 * TRC  = [60 -]        TRC = Row cycle delay
 * TRAS = [42 100000]   TRAS = Self refresh time
 * TRCD = [15 -]        TRCD = Row to column delay ~ Activate Row to Read Command
 * TCCD = [1 -]
 * TRP  = [15 -]        TRP = Row precharge delay ~ Precharge to Activate Row
 * TCCD = [1 -]         TWR = Recovery delay
 *                      TXSR = Exit Self-refresh delay
 * CAS Latency  2/3     TMRD = Load Mode Register to Active
 * Burst Length 1 2 4 8 and FULL Page
 *
 *
 */
#include "board.h"
#include "drv_sdram.h"
#include "drv_ltdc.h"

#define DRV_DEBUG

#define DBG_TAG              "drv.sdram"
#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

void delay(rt_uint32_t us)
{
    while(us--);
}

void sdram_low_init(void)
{
 // configuration clock for sdram and pins

    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_FMC_CLK_ENABLE();                 //使能FMC时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();               //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();               //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();               //使能GPIOE时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟

    //初始化PC0,2,3
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;         //高速
    GPIO_Initure.Alternate=GPIO_AF12_FMC;       //复用为FMC
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化


    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD,&GPIO_Initure); //初始化PD0,1,8,9,10,14,15

    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); //初始化PE0,1,7,8,9,10,11,12,13,14,15

    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure); //初始化PF0,1,2,3,4,5,11,12,13,14,15

    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure); //初始化PG0,1,2,4,5,8,15
}

int sdram_init(void)
{
    sdram_low_init();

    struct fmc_regs *base = (struct fmc_regs*)(0x52004000);

    if (base->bcr1 & (0x1 << 31))
        base->bcr1 &=~(0x1 << 31);

    base->sdcr1 = 0;
    base->sdtr1 = 0;
    base->sdcmr = 0;
    base->sdrtr = 0;

    base->sdcr1 = (0x0 << RPIPE_SHIFT | 0x0 << RBURST_SHIFT | 0x02 << SDCLK_SHIFT \
                   | 0x0 << WP_SHIFT | 0x2 << CAS_SHIFT | 0x1 <<NB_SHIFT | 0x1 << MWID_SHIFT\
                   | 0x2 << NR_SHIFT | 0x1 << NC_SHIFT);
    base->sdtr1 = (0x2 << TRCD_SHIFT | 0x2 << TRP_SHIFT | 0x2 << TWR_SHIFT | 0x6 << TRC_SHIFT \
                   | 0x6 << TRAS_SHIFT | 0x8 << TXSR_SHIFT | 0x2 << TMRD_SHIFT);

    base->sdcmr = (0x1 << CTB1_SHIFT | 0x1 << MODE_SHIFT);
    delay(500000);

    base->sdcmr = (0x1 << CTB1_SHIFT | 0x2 << MODE_SHIFT);
    base->sdcmr = (0x1 << CTB1_SHIFT | 0x3 << MODE_SHIFT | 0x7 << NRFS_SHIFT);
    base->sdcmr = (0x1 << CTB1_SHIFT | ((0x0 << 0 | 0x2 << 4) << MRD_SHIFT) | 0x4 << MODE_SHIFT );
    base->sdrtr = (667<<1);
    base->bcr1 |=(0x1 << 31);

    return 0;
}
