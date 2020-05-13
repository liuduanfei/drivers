# drivers
stm32h7 drivers for rtt-studio
1.生成汇编文件，方便调试代码。
在`build steps->post-build step`中
添加`${cross_prefix}objdump -D rtthread.elf > rtthread.dis`即可。