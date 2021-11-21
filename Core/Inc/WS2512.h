//
// Created by 58286 on 2021/11/21.
//

#ifndef BOARD_B_CORE_INC_WS2512_H_
#define BOARD_B_CORE_INC_WS2512_H_
#include "main.h"

#define ZERO 21
#define ONE 52

#define RED (50<<8)
#define BLUE 50

#define OFF_ID 0
#define ON_ID 1
#define ARROW_ID 2

#define NO1 0x00
#define NO2 0x01

#define ALL_OFF 0x00
#define ALL_ON 0x02
#define TOP_OFF 0x04
#define TOP_ON 0x06
#define BOT_OFF 0x08
#define BOT_ON 0x0A
#define MID_ON 0x0C
#define MID_OFF 0x0E
#define MID_ARROW 0x10

#define DMA_TOP1 hdma_tim1_ch1
#define DMA_TOP2 hdma_tim1_ch4_trig_com
#define DMA_MID1 hdma_tim2_ch1
#define DMA_MID2 hdma_tim2_ch2_ch4
#define DMA_BOT1 hdma_tim2_ch3
#define DMA_BOT2 hdma_tim3_ch1_trig

#define TIM_TOP1 htim1
#define TIM_TOP2 htim1
#define TIM_MID1 htim2
#define TIM_MID2 htim2
#define TIM_BOT1 htim2
#define TIM_BOT2 htim3

#define TIM_NUM_TOP1 1
#define TIM_NUM_TOP2 1
#define TIM_NUM_MID1 2
#define TIM_NUM_MID2 2
#define TIM_NUM_BOT1 2
#define TIM_NUM_BOT2 3

#define CHANNEL_TOP1 TIM_CHANNEL_1
#define CHANNEL_TOP2 TIM_CHANNEL_4
#define CHANNEL_MID1 TIM_CHANNEL_1
#define CHANNEL_MID2 TIM_CHANNEL_2
#define CHANNEL_BOT1 TIM_CHANNEL_3
#define CHANNEL_BOT2 TIM_CHANNEL_1

#define TOP1 1
#define TOP2 2
#define BOT1 3
#define BOT2 4
#define MID1 5
#define MID2 6

#define TOP_SIZE 30
#define BOT_SIZE 30
#define MID_SIZE 256

#define ON 1
#define OFF 0

typedef struct{
    uint16_t top1;
    uint16_t top2;
    uint16_t mid1;
    uint16_t mid2;
    uint16_t bot1;
    uint16_t bot2;
    uint8_t mid1_arrow;
    uint8_t mid2_arrow;
}LED_StateStructure;

void init_WS2512(void);
void setRGB(uint32_t rgb);
void setLED_State(uint8_t LED_type,uint8_t state);
void setMidArrow(uint8_t LED_type,uint8_t state);
//desData length = 24*length
void transferToLED(const uint8_t *data,uint16_t length,uint8_t* dstData);

#endif //BOARD_B_CORE_INC_WS2512_H_
