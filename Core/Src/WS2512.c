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
  memcpy(top1_LED,LED_on,24);
  memcpy(top2_LED,LED_on,24);
  memcpy(mid1_LED,LED_on,24);
  memcpy(mid2_LED,LED_on,24);
  memcpy(bot1_LED,LED_on,24);
  memcpy(bot2_LED,LED_on,24);
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
  switch (LED_type) {
    case TOP1:
      while(LED_State.top1||LED_State.top2);
      HAL_TIM_PWM_Start_DMA(&TIM_TOP1,CHANNEL_TOP1,(uint32_t*)(state==ON?top1_LED:LED_off),24);
      LED_State.top1 = TOP_SIZE;break;
    case TOP2:
      while(LED_State.top2||LED_State.top1);
      HAL_TIM_PWM_Start_DMA(&TIM_TOP2,CHANNEL_TOP2,(uint32_t*)(state==ON?top2_LED:LED_off),24);
      LED_State.top2 = TOP_SIZE;break;
    case MID1:
      while(LED_State.mid1||LED_State.mid2||LED_State.bot1);
      HAL_TIM_PWM_Start_DMA(&TIM_MID1,CHANNEL_MID1,(uint32_t*)(state==ON?mid1_LED:LED_off),24);
      LED_State.mid1 = MID_SIZE;break;
    case MID2:
      while(LED_State.mid1||LED_State.mid2||LED_State.bot1);
      HAL_TIM_PWM_Start_DMA(&TIM_MID2,CHANNEL_MID2,(uint32_t*)(state==ON?mid2_LED:LED_off),24);
      LED_State.mid2 = MID_SIZE;break;
    case  BOT1:
      while(LED_State.mid1||LED_State.mid2||LED_State.bot1);
      HAL_TIM_PWM_Start_DMA(&TIM_BOT1,CHANNEL_BOT1,(uint32_t*)(state==ON?bot1_LED:LED_off),24);
      LED_State.bot1 = BOT_SIZE;break;
    case BOT2:
      while(LED_State.bot2);
      HAL_TIM_PWM_Start_DMA(&TIM_BOT2,CHANNEL_BOT2,(uint32_t*)(state==ON?bot2_LED:LED_off),24);
      LED_State.bot2 = BOT_SIZE;break;
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
    if(state==ON&&LED_State.mid1_arrow==0){
      LED_State.mid1_arrow = 1;
      HAL_TIM_Base_Start_IT(&htim4);
    }else{
      LED_State.mid1_arrow = 0;
    }
  }else if(LED_type == MID2){
    if(state==ON&&LED_State.mid2_arrow==0){
      LED_State.mid2_arrow = 1;
      HAL_TIM_Base_Start_IT(&htim4);
    }else{
      LED_State.mid2_arrow = 0;
    }
  }
}

uint8_t setLED(uint8_t LED_type,uint8_t state)
{
  switch (LED_type) {
    case TOP1:
    case TOP2:
    case BOT1:
    case BOT2:
      if(state==ON||state==OFF)
        setLED_State(LED_type,state);
      else return 0;
      break;
    case MID1:
    case MID2:
      if(state==ON||state==OFF)
        setLED_State(LED_type,state);
      else if(state==ARROW)
        setMidArrow(LED_type,ON);
      else if(state==HALT)
        setMidArrow(LED_type,OFF);
      else return 0;
      break;
    default:return 0;
  }
  return 1;
}

void LED_FSM(void)
{
  static uint8_t data;
  if(getQueue(data_queue,&data)){
    if((data&1)==NO1){
      switch (data&0xFE) {
        case ALL_ON:
          setMidArrow(MID1,OFF);
          setLED_State(TOP1,ON);
          setLED_State(MID1,ON);
          setLED_State(BOT1,ON);break;
        case ALL_OFF:
          setMidArrow(MID1,OFF);
          setLED_State(MID1,OFF);
          setLED_State(TOP1,OFF);
          setLED_State(BOT1,OFF);break;
        case TOP_ON:
          setLED_State(TOP1,ON);break;
        case TOP_OFF:
          setLED_State(TOP1,OFF);break;
        case MID_ON:
          setMidArrow(MID1,OFF);
          setLED_State(MID1,ON);break;
        case MID_OFF:
          setMidArrow(MID1,OFF);
          setLED_State(MID1,OFF);break;
        case MID_ARROW:
          setMidArrow(MID1,ON);break;
        case BOT_ON:
          setLED_State(BOT1,ON);break;
        case BOT_OFF:
          setLED_State(BOT1,OFF);break;
        case ARROW_ON:
          setLED_State(BOT1,ON);
          setMidArrow(MID1,ON);break;
      }
    }else if((data&1)==NO2){
      switch (data&0xFE) {
        case ALL_ON:
          setMidArrow(MID2,OFF);
          setLED_State(TOP2,ON);
          setLED_State(MID2,ON);
          setLED_State(BOT2,ON);break;
        case ALL_OFF:
          setMidArrow(MID2,OFF);
          setLED_State(TOP2,OFF);
          setLED_State(MID2,OFF);
          setLED_State(BOT2,OFF);break;
        case TOP_ON:
          setLED_State(TOP2,ON);break;
        case TOP_OFF:
          setLED_State(TOP2,OFF);break;
        case MID_ON:
          setLED_State(MID2,OFF);
          setLED_State(MID2,ON);break;
        case MID_OFF:
          setLED_State(MID2,OFF);
          setLED_State(MID2,OFF);break;
        case MID_ARROW:
          setMidArrow(MID2,ON);break;
        case BOT_ON:
          setLED_State(BOT2,ON);break;
        case BOT_OFF:
          setLED_State(BOT2,OFF);break;
        case ARROW_ON:
          setLED_State(BOT2,ON);
          setMidArrow(MID2,ON);break;
      }
    }
  }
}

void test(void)
{
  HAL_TIM_PWM_Start_DMA(&TIM_MID1,CHANNEL_MID1,(uint32_t*)mid1_LED,24);
  LED_State.mid1 = MID_SIZE;
}