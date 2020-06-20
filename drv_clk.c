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
#include "drv_common.h"

#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

void system_clock_config(int target_freq_mhz)
{
    HAL_StatusTypeDef ret=HAL_OK;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    MODIFY_REG(PWR->CR3,PWR_CR3_SCUEN, 0);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}

    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState=RCC_HSE_ON;
    RCC_OscInitStruct.HSIState=RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState=RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;

    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    ret=HAL_RCC_OscConfig(&RCC_OscInitStruct);

    if(ret!=HAL_OK) while(1);

    RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|\
                                RCC_CLOCKTYPE_HCLK |\
                                RCC_CLOCKTYPE_D1PCLK1 |\
                                RCC_CLOCKTYPE_PCLK1 |\
                                RCC_CLOCKTYPE_PCLK2 |\
                                RCC_CLOCKTYPE_D3PCLK1);

    RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider=RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider=RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider=RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider=RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider=RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider=RCC_APB4_DIV4;
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
    if(ret!=HAL_OK) while(1);

    RCC_PeriphCLKInitTypeDef PeriphClkIniture;

    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkIniture.PLL3.PLL3M = 5;
    PeriphClkIniture.PLL3.PLL3N = 160;
    PeriphClkIniture.PLL3.PLL3P = 2;
    PeriphClkIniture.PLL3.PLL3Q = 2;
    PeriphClkIniture.PLL3.PLL3R = 24;
    ret=HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture);
    if(ret!=HAL_OK) while(1);

    __HAL_RCC_CSI_ENABLE() ;
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;
    HAL_EnableCompensationCell();
}
int clock_information(void)
{
    LOG_I("System Clock information");
    LOG_I("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_I("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_I("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_I("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());

    return RT_EOK;
}
INIT_BOARD_EXPORT(clock_information);

void clk_init(char *clk_source, int source_freq, int target_freq)
{
    system_clock_config(target_freq);
}
