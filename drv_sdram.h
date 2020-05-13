#ifndef _DRV_SDRAM_H
#define _DRV_SDRAM_H


#define FMC_ENABLE    (0x1<<31)

struct fmc_regs {
    /* 0x0 */
    volatile rt_uint32_t bcr1;   /* NOR/PSRAM Chip select control register 1 */
    volatile rt_uint32_t btr1;   /* SRAM/NOR-Flash Chip select timing register 1 */
    volatile rt_uint32_t bcr2;   /* NOR/PSRAM Chip select Control register 2 */
    volatile rt_uint32_t btr2;   /* SRAM/NOR-Flash Chip select timing register 2 */
    volatile rt_uint32_t bcr3;   /* NOR/PSRAMChip select Control register 3 */
    volatile rt_uint32_t btr3;   /* SRAM/NOR-Flash Chip select timing register 3 */
    volatile rt_uint32_t bcr4;   /* NOR/PSRAM Chip select Control register 4 */
    volatile rt_uint32_t btr4;   /* SRAM/NOR-Flash Chip select timing register 4 */
    volatile rt_uint32_t reserved1[24];

    /* 0x80 */
    volatile rt_uint32_t pcr;    /* NAND Flash control register */
    volatile rt_uint32_t sr;     /* FIFO status and interrupt register */
    volatile rt_uint32_t pmem;   /* Common memory space timing register */
    volatile rt_uint32_t patt;   /* Attribute memory space timing registers  */
    volatile rt_uint32_t reserved2[1];
    volatile rt_uint32_t eccr;   /* ECC result registers */
    volatile rt_uint32_t reserved3[27];

    /* 0x104 */
    volatile rt_uint32_t bwtr1;  /* SRAM/NOR-Flash write timing register 1 */
    volatile rt_uint32_t reserved4[1];
    volatile rt_uint32_t bwtr2;  /* SRAM/NOR-Flash write timing register 2 */
    volatile rt_uint32_t reserved5[1];
    volatile rt_uint32_t bwtr3;  /* SRAM/NOR-Flash write timing register 3 */
    volatile rt_uint32_t reserved6[1];
    volatile rt_uint32_t bwtr4;  /* SRAM/NOR-Flash write timing register 4 */
    volatile rt_uint32_t reserved7[8];

    /* 0x140 */
    volatile rt_uint32_t sdcr1;  /* SDRAM Control register 1 */
    volatile rt_uint32_t sdcr2;  /* SDRAM Control register 2 */
    volatile rt_uint32_t sdtr1;  /* SDRAM Timing register 1 */
    volatile rt_uint32_t sdtr2;  /* SDRAM Timing register 2 */
    volatile rt_uint32_t sdcmr;  /* SDRAM Mode register */
    volatile rt_uint32_t sdrtr;  /* SDRAM Refresh timing register */
    volatile rt_uint32_t sdsr;   /* SDRAM Status register */
};

#define RPIPE_SHIFT  13
#define RBURST_SHIFT 12
#define SDCLK_SHIFT  10
#define WP_SHIFT     9
#define CAS_SHIFT    7
#define NB_SHIFT     6
#define MWID_SHIFT   4
#define NR_SHIFT     2
#define NC_SHIFT     0

#define TRCD_SHIFT   24
#define TRP_SHIFT    20
#define TWR_SHIFT    16
#define TRC_SHIFT    12
#define TRAS_SHIFT   8
#define TXSR_SHIFT   4
#define TMRD_SHIFT   0


#define MRD_SHIFT    9
#define NRFS_SHIFT   5
#define CTB1_SHIFT   4
#define CTB2_SHIFT   3
#define MODE_SHIFT   0


#define REIE_SHIFT   14
#define COUNT_SHIFT  1
#define CRE_SHIFT    0

#define MODES2_SHIFT 3
#define MODES1_SHIFT 1
#define RE_SHIFT     0


void sdram_low_init(void);
int sdram_init(void);
#define FMC_SDCR1 (*(volatile rt_unit32_t*)(FMC_BASE_ADDR + 0x140))



#endif /* _DRV_SDRAM_H */
