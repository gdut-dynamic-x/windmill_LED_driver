//
// Created by 58286 on 2021/11/20.
//

#ifndef FANBBOARD_CORE_INC_QUEUE_H_
#define FANBBOARD_CORE_INC_QUEUE_H_
#include "main.h"
#include "memory.h"

#define MAX_QUEUE_SIZE 128
#define HEAD_SIZE 6

typedef void* QueueHandle_t;

typedef struct {
    uint8_t isUsed;
    uint8_t length;
    uint8_t data_size;
    uint8_t pointer;
    uint8_t first;
    uint8_t amount;
}QueueHead;

void initMainQueue(void);
QueueHandle_t createQueue(uint16_t length,uint16_t data_size);
uint8_t addQueue(QueueHandle_t handle,void* data);
uint8_t getQueue(QueueHandle_t handle,void* data);

#endif //FANBBOARD_CORE_INC_QUEUE_H_
