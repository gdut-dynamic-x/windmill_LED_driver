//
// Created by 58286 on 2021/11/21.
//
#include "WS2512.h"
#include "queue.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

QueueHandle_t data_queue;
LED_StateStructure LED_State;
uint8_t LED_on[24];
uint8_t LED_off[24];

uint8_t top1_LED[24];
uint8_t top2_LED[24];
uint8_t bot1_LED[24];
uint8_t bot2_LED[24];
uint8_t mid1_LED[24];
uint8_t mid2_LED[24];

uint8_t arrows[4608];
uint8_t *mid1_arrow;
uint8_t *mid2_arrow;

uint8_t arrow[48] = {
    1,0,0,0,0,0,0,1,
    1,1,0,0,0,0,1,1,
    1,1,1,0,0,1,1,1,
    0,1,1,1,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0
};


void init_WS2512(void)
{
  data_queue = createQueue(10,sizeof(uint8_t));
  LED_State.bot1 = 0;
  LED_State.bot2 = 0;
  LED_State.mid1 = 0;
  LED_State.mid2 = 0;
  LED_State.top2 = 0;
  LED_State.top1 = 0;
  for(uint8_t i=0;i<24;i++){
    LED_on[i] = ZERO;
    LED_off[i] = ZERO;
  }
  setRGB(RED);
  mid1_arrow = arrows;
  mid2_arrow = arrows+2304;
  transferToLED(arrow,48,mid1_arrow);
  transferToLED(arrow,48,mid2_arrow);
}

void setRGB(uint32_t rgb)
{
  for(uint8_t i=0;i<24;i++){
    if((rgb>>i)&1){
      LED_on[23-i] = ONE;
    }else{
      LED_on[23-i] = ZERO;
    }
  }
}

void setLED_State(uint8_t LED_type,uint8_t state)
{
  uint8_t* led_state;
  if(state == ON) led_state = LED_on;
  else led_state = LED_off;
  switch (LED_type) {
    case TOP1:
      HAL_TIM_PWM_Start_DMA(&TIM_TOP1,CHANNEL_TOP1,(uint32_t*)led_state,24);
      LED_State.top1 = TOP_SIZE-1;break;
    case TOP2:
      HAL_TIM_PWM_Start_DMA(&TIM_TOP2,CHANNEL_TOP2,(uint32_t*)led_state,24);
      LED_State.top2 = TOP_SIZE-1;break;
    case MID1:
      HAL_TIM_PWM_Start_DMA(&TIM_MID1,CHANNEL_MID1,(uint32_t*)led_state,24);
      LED_State.mid1 = MID_SIZE-1;break;
    case MID2:
      HAL_TIM_PWM_Start_DMA(&TIM_MID2,CHANNEL_MID2,(uint32_t*)led_state,24);
      LED_State.mid2 = MID_SIZE-1;break;
    case  BOT1:
      HAL_TIM_PWM_Start_DMA(&TIM_BOT1,CHANNEL_BOT1,(uint32_t*)led_state,24);
      LED_State.bot1 = BOT_SIZE-1;break;
    case BOT2:
      HAL_TIM_PWM_Start_DMA(&TIM_BOT2,CHANNEL_BOT2,(uint32_t*)led_state,24);
      LED_State.bot2 = BOT_SIZE-1;break;
    default:break;
  }
}

void transferToLED(const uint8_t * data,uint16_t length,uint8_t* dstData)
{
  for(uint16_t i=0;i<length;i++){
    if(data[i]){
      memcpy(dstData+24*i,LED_on,24);
    }else{
      memcpy(dstData+24*i,LED_off,24);
    }
  }
}

void setMidArrow(uint8_t LED_type,uint8_t state)
{
  if(LED_type == MID1){
    LED_State.mid1_arrow = 1;
    HAL_TIM_Base_Start_IT(&htim4);
  }else if(LED_type == MID2){
    LED_State.mid2_arrow = 1;
    HAL_TIM_Base_Start_IT(&htim4);
  }
}