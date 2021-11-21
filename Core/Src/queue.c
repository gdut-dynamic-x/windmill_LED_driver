//
// Created by 58286 on 2021/11/20.
//
#include "queue.h"

uint8_t mainQueue[MAX_QUEUE_SIZE];
uint8_t mainPointer = HEAD_SIZE;

#define findHead() mainPointer - HEAD_SIZE
#define getHead(p) ((QueueHead*)(mainQueue+p))[0]
#define resetMainPointer() mainPointer = HEAD_SIZE

void initMainQueue(void)
{
  QueueHead head = { 0,MAX_QUEUE_SIZE - HEAD_SIZE,0,HEAD_SIZE,0 };
  ((QueueHead*)mainQueue)[0] = head;
}

QueueHandle_t createQueue(uint16_t length, uint16_t data_size)
{
  resetMainPointer();
  uint8_t p = 0;
  while (getHead(p).isUsed != 0) {
    mainPointer += getHead(p).length + HEAD_SIZE;
    if (mainPointer >= MAX_QUEUE_SIZE) return NULL;
    p = findHead();
  }
  getHead(p + length + HEAD_SIZE).length = getHead(p).length - length - HEAD_SIZE;
  getHead(p + length + HEAD_SIZE).isUsed = 0;

  getHead(p).isUsed = 1;
  getHead(p).length = length;
  getHead(p).amount = 0;
  getHead(p).data_size = data_size;
  getHead(p).pointer = 0;
  getHead(p).first = 0;

  return mainQueue + mainPointer;
}

uint8_t addQueue(QueueHandle_t handle, void* data)
{
  QueueHead* head = (QueueHead*)((uint8_t*)handle - HEAD_SIZE);
  if (head->amount == head->length) return 0;
  memcpy((uint8_t*)handle + head->pointer, data, head->data_size);
  head->pointer += head->data_size;
  if (head->pointer >= head->length * head->data_size) head->pointer = 0;
  head->amount++;
  return 1;
}

uint8_t getQueue(QueueHandle_t handle, void* data)
{
  QueueHead* head = (QueueHead*)((uint8_t*)handle - HEAD_SIZE);
  if (head->amount == 0)return  0;
  memcpy(data, (uint8_t*)handle + head->first, head->data_size);
  head->amount--;
  head->first += head->data_size;
  if (head->first >= head->length * head->data_size) head->first = 0;
  return 1;
}