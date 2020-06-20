# RT-Thread的初始化顺序
1. 上电执行汇编函数 复位中断 `Reset_Handler`

2. 复位中断函数中，拷贝`bss`段，清除`bss`段然后跳转到 `entry` 函数

3. entry函数中，调用`rtthread_startup`该函数首先调用`rt_hw_board_init`函数，进行硬件的初始化，最后初始化 `INIT_BOARD_EXPORT(fn)`宏包裹的函数。

4. 然后创建 `main_thread_entry`，也就是main线程。

5. main线程中调用组件初始化函数。````

6. 组件顺序如下：
   `INIT_PREV_EXPORT(fn)`

   `INIT_DEVICE_EXPORT(fn)`

   `INIT_COMPONENT_EXPORT(fn)`

   `INIT_ENV_EXPORT(fn)`

   `INIT_APP_EXPORT(fn)`

7. 然后调用`main`函数。

注：同优先级自动初始化宏包裹的函数初始化顺序不确定。

# Drivers
stm32h7 drivers for rtt-studio

需要注意的问题：

**多块ram**

H7芯片多内存的问题，像sdmmc1，并不能访问DTCM区，用这个区做主ram，会导致数据访问出错。

需要在`board.c`或`board.h`文件中修改相应的参数，并修改链接脚本。

**TCM 区**
TCM : Tightly-Coupled Memory 紧密耦合内存 。 ITCM 用于运行指令，也就是程序代码，DTCM
用于数据存取，特点是跟内核速度一样，而片上 RAM 的速度基本都达不到这个速度，所以有降频处
理。
速度：400MHz。
DTCM 地址：0x2000 0000，大小 128KB。
ITCM 地址：0x0000 0000，大小 64KB。
**AXI SRAM 区**
位于 D1 域，数据带宽是 64bit，挂在 AXI 总线上。 除了 D3 域中的 BDMB 主控不能访问，其它都可
以访问此 RAM 区。
速度：200MHz。
地址：0x2400 0000，大小 512KB。
用途：用途不限，可以用于用户应用数据存储或者 LCD 显存。
**SRAM1，SRAM2 和 SRAM3 区**
位于 D2 域，数据带宽是 32bit，挂在 AHB 总线上。除了 D3 域中的 BDMB 主控不能访问这三块 SRAM，
其它都可以访问这几个 RAM 区。
速度：200MHz。
SRAM1：地址 0x3000 0000，大小 128KB，用途不限，可用于 D2 域中的 DMA 缓冲，也可以当D1 域断电后用于运行程序代码。
SRAM2：地址 0x3002 0000，大小 128KB，用途不限，可用于 D2 域中的 DMA 缓冲，也可以用于用户数据存取。
SRAM3：地址 0x3004 0000，大小 32KB，用途不限，主要用于以太网和 USB 的缓冲。

**SRAM4 区**
位于 D3 域，数据带宽是 32bit，挂在 AHB 总线上，大部分主控都能访这块 SRAM 区。
速度：200MHz。
地址：0x3800 0000，大小 64KB。
用途：用途不限，可以用于 D3 域中的 DMA 缓冲，也可以当 D1 和 D2 域进入 DStandby 待机方式
后，继续保存用户数据。
 **Backup SRAM 区**
备份 RAM 区，位于 D3 域，数据带宽是 32bit，挂在 AHB 总线上，大部分主控都能访问这块 SRAM
区。
速度：200MHz。
地址：0x3880 0000，大小 4KB。
用途：用途不限，主要用于系统进入低功耗模式后，继续保存数据（Vbat 引脚外接电池）。 

-----------------------------------------------------------------------------------------------------------------------------------------------------------

**外设多时钟源**

外设有多种时钟源选择，且分为外设时钟和kernel时钟。

驱动中不同外设的时钟源选如下：

-gpio

-uart

-ltdc

地址：0x50001000 - 0x50001FFF

时钟：

最高：66MHz

来源：pll3_r_ck

RCC_APB3RSTR    bit3 复位ltdc

RCC_APB3ENR      bit3  使能ltdc 提供像素时钟

RCC_APB3LPENR  bit3 csleep模式下的控制

-sdmmc

-qspi

-fmc

-rtc

-wdg

-sai1

地址：0x40015800 - 0x40015BFF

时钟：

最高：133Mhz

时钟源：

pll1_q_ck（默认）

pll2_p_ck

pll3_p_ck

I2S_CKIN

per_ck

为了不跟ltdc冲突，选择pll2_p_ck

RCC_D2CCIP1R  bit8-6选择时钟源

000: pll1_q_ck clock selected as SAI2 and SAI3 kernel clock (default after reset)
001: pll2_p_ck clock selected as SAI2 and SAI3 kernel clock
010: pll3_p_ck clock selected as SAI2 and SAI3 kernel clock
011: I2S_CKIN clock selected as SAI2 and SAI3 kernel clock
100: per_ck clock selected as SAI2 and SAI3 kernel clock 





1.生成汇编文件，方便调试代码。
在build steps->post-build step
添加
${cross_prefix}objdump -D rtthread.elf > rtthread.dis