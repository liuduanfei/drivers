#ifndef _DRV_LTDC_H
#define _DRV_LTDC_H

#include <rtthread.h>

struct stm32_ltdc_reg
{
    volatile rt_uint32_t reserved1[2]; /* 0x0~0x04 */
    volatile rt_uint32_t sscr;         /* LTDC synchronization size configuration register */
    volatile rt_uint32_t bpcr;         /* LTDC back porch configuration register */
    volatile rt_uint32_t awcr;         /* LTDC active width configuration register */
    volatile rt_uint32_t twcr;         /* LTDC total width configuration register */
    volatile rt_uint32_t gcr;          /* LTDC global control register */
    volatile rt_uint32_t reserved2[2]; /* 0x1C~0x20 */
    volatile rt_uint32_t srcr;         /* LTDC shadow reload configuration register */
    volatile rt_uint32_t reserved3;    /* 0x1C~0x20 */
    volatile rt_uint32_t bccr;         /* LTDC background color configuration register */
    volatile rt_uint32_t reserved4;    /* 0x30 */
    volatile rt_uint32_t ier;          /* LTDC interrupt enable register */
    volatile rt_uint32_t isr;          /* LTDC interrupt status register */
    volatile rt_uint32_t icr;          /* LTDC Interrupt Clear Register */
    volatile rt_uint32_t lipcr;        /* LTDC line interrupt position configuration register */
    volatile rt_uint32_t cpsr;         /* LTDC current position status register */
    volatile rt_uint32_t cdsr;         /* LTDC current display status register  */

    volatile rt_uint32_t reserved5[14]; /* 0x4C~0x80 */

    volatile rt_uint32_t l1cr;         /* LTDC layer 1 control register */
    volatile rt_uint32_t l1whpcr;      /* LTDC layer 1 window horizontal position configuration register */
    volatile rt_uint32_t l1wvpcr;      /* LTDC layer 1 window vertical position configuration register */
    volatile rt_uint32_t l1ckcr;       /* LTDC layer 1 color keying configuration register */
    volatile rt_uint32_t l1pfcr;       /* LTDC layer 1 pixel format configuration register */
    volatile rt_uint32_t l1cacr;       /* LTDC layer 1 constant alpha configuration register */
    volatile rt_uint32_t l1dccr;       /* LTDC layer 1 default color configuration register */
    volatile rt_uint32_t l1bfcr;       /* LTDC layer 1 blending factors configuration register */
    volatile rt_uint32_t reserved6[2];
    volatile rt_uint32_t l1cfbar;      /* LTDC layer 1 color frame buffer address register */
    volatile rt_uint32_t l1cfblr;      /* LTDC layer 1 color frame buffer length register */
    volatile rt_uint32_t l1cfblnr;     /* LTDC layer 1 color frame buffer line number register */
    volatile rt_uint32_t reserved7[3];
    volatile rt_uint32_t l1clutwr;     /* LTDC layer 1 CLUT write register */

    volatile rt_uint32_t reserved8[63];

    volatile rt_uint32_t l2cr;
    volatile rt_uint32_t l2whpcr;
    volatile rt_uint32_t l2wvpcr;
    volatile rt_uint32_t l2ckcr;
    volatile rt_uint32_t l2pfcr;
    volatile rt_uint32_t l2cacr;
    volatile rt_uint32_t l2dccr;
    volatile rt_uint32_t l2bfcr;
    volatile rt_uint32_t reserved9[2];
    volatile rt_uint32_t l2cfbar;
    volatile rt_uint32_t l2cfblr;
    volatile rt_uint32_t l2cfblnr;
    volatile rt_uint32_t reserved10[3];
    volatile rt_uint32_t l2clutwr;
};


int ltdc_init(void);
void clean_scr(void);
void back_light_on(void);
void draw_point(rt_uint16_t x, rt_uint16_t y, uint16_t color);

#endif /* _DRV_LTDC_H */
