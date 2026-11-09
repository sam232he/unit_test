/**
 * @file    canfd_defines.h
 *
 * @author  allahmoradi
 *
 * @date    Jul 4, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  canfd_defines
 *
 * @brief   
 *
 * @details 
 *
 */

#ifndef SUBM_CANFD_INTERFACE_CANFD_MANAGE_DEFINES_H_
#define SUBM_CANFD_INTERFACE_CANFD_MANAGE_DEFINES_H_
/*================== Includes ===============================================*/
#include <stdint.h>
#include <stdbool.h>
/*================== Macros and Definitions =================================*/
#ifndef NULL
#define NULL ((void *)0x0)
#endif

typedef enum
{
    CANFD_500K_1M,
    CANFD_500K_2M,
    CANFD_500K_3M,
    CANFD_500K_4M,
    CANFD_500K_5M,
    CANFD_500K_6M7,
    CANFD_500K_8M,
    CANFD_500K_10M,
    CANFD_250K_500K,
    CANFD_250K_833K,
    CANFD_250K_1M,
    CANFD_250K_1M5,
    CANFD_250K_2M,
    CANFD_250K_3M,
    CANFD_250K_4M,
    CANFD_1000K_4M,
    CANFD_1000K_8M,
    CANFD_125K_500K
} CANFD_BITTIME_SETUP_e;
typedef enum
{
    CANFD_OSCCLK_40M,
    CANFD_OSCCLK_20M,
    CANFD_OSCCLK_10M
} CANFD_SYSCLK_SPEED_e;
typedef enum
{
    CANFD_FIFO_CH0,   // CAN_TXQUEUE_CH0
    CANFD_FIFO_CH1,
    CANFD_FIFO_CH2,
    CANFD_FIFO_CH3,
    CANFD_FIFO_CH4,
    CANFD_FIFO_CH5,
    CANFD_FIFO_CH6,
    CANFD_FIFO_CH7,
    CANFD_FIFO_CH8,
    CANFD_FIFO_CH9,
    CANFD_FIFO_CH10,
    CANFD_FIFO_CH11,
    CANFD_FIFO_CH12,
    CANFD_FIFO_CH13,
    CANFD_FIFO_CH14,
    CANFD_FIFO_CH15,
    CANFD_FIFO_CH16,
    CANFD_FIFO_CH17,
    CANFD_FIFO_CH18,
    CANFD_FIFO_CH19,
    CANFD_FIFO_CH20,
    CANFD_FIFO_CH21,
    CANFD_FIFO_CH22,
    CANFD_FIFO_CH23,
    CANFD_FIFO_CH24,
    CANFD_FIFO_CH25,
    CANFD_FIFO_CH26,
    CANFD_FIFO_CH27,
    CANFD_FIFO_CH28,
    CANFD_FIFO_CH29,
    CANFD_FIFO_CH30,
    CANFD_FIFO_CH31,
    CANFD_FIFO_TOTAL_CHANNELS
} CANFD_CAN_FIFO_CHANNEL_e;
typedef enum
{
    CANFD_FILTER0,
    CANFD_FILTER1,
    CANFD_FILTER2,
    CANFD_FILTER3,
    CANFD_FILTER4,
    CANFD_FILTER5,
    CANFD_FILTER6,
    CANFD_FILTER7,
    CANFD_FILTER8,
    CANFD_FILTER9,
    CANFD_FILTER10,
    CANFD_FILTER11,
    CANFD_FILTER12,
    CANFD_FILTER13,
    CANFD_FILTER14,
    CANFD_FILTER15,
    CANFD_FILTER16,
    CANFD_FILTER17,
    CANFD_FILTER18,
    CANFD_FILTER19,
    CANFD_FILTER20,
    CANFD_FILTER21,
    CANFD_FILTER22,
    CANFD_FILTER23,
    CANFD_FILTER24,
    CANFD_FILTER25,
    CANFD_FILTER26,
    CANFD_FILTER27,
    CANFD_FILTER28,
    CANFD_FILTER29,
    CANFD_FILTER30,
    CANFD_FILTER31,
    CANFD_FILTER_TOTAL,
} CANFD_CAN_FILTER_e;
typedef enum
{
    CANFD_MSG_FRAME_STD,
    CANFD_MSG_FRAME_EXT
} CANFD_MSG_FRAME_TYPE_e;
typedef struct
{
    uint32_t id;
    uint32_t mask;
    CANFD_MSG_FRAME_TYPE_e type;
    bool enable;
} CANFD_RX_FILTER_s;
typedef struct
{
    CANFD_RX_FILTER_s rx_filter[32];
    uint32_t spi_base;
    uint32_t (* gpio_read_f)(uint32_t);
    float sys_clk;
    void (*spi_write)(uint32_t, uint16_t);
    void (*spi_cs_control)(uint32_t, bool);
    uint16_t (*spi_read)(uint32_t);
    CANFD_BITTIME_SETUP_e bittime;
    uint16_t rx_gpio_pin;
    bool iso_mode;
    CANFD_SYSCLK_SPEED_e osc_clk;
} CANFD_CFG_s;

typedef struct
{
    uint16_t data_len;
    uint16_t msg_data[8];
    uint32_t message_id;
    bool data_ack;
    CANFD_MSG_FRAME_TYPE_e frame_type;
    bool update_flg;
} CANFD_MSG_s;
typedef enum
{
    canfd_interface_tx_task_wait_s,
    canfd_interface_tx_task_read_data_s,
    canfd_interface_tx_task_send_data_s,
} canfd_interface_tx_task_STATE_e;

typedef enum {
    CANFD_ERROR_FREE_STATE = 0,
    CANFD_ERROR_ALL = 0x3F,
    CANFD_TX_RX_WARNING_STATE = 0x01,
    CANFD_RX_WARNING_STATE = 0x02,
    CANFD_TX_WARNING_STATE = 0x04,
    CANFD_RX_BUS_PASSIVE_STATE = 0x08,
    CANFD_TX_BUS_PASSIVE_STATE = 0x10,
    CANFD_TX_BUS_OFF_STATE = 0x20,
    CANFD_TX_ERR_FIFO_FULL = 0x40
}CANFD_ERROR_STATE_e;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/

#endif /* SUBM_CANFD_INTERFACE_CANFD_MANAGE_DEFINES_H_ */
/*================== END MODULE ==============================================*/
