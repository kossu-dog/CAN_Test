/*
 * can_test.h
 *
 *  Created on: 15Jul.,2020
 *      Author: micha
 */

#ifndef CAN_TEST_H_
#define CAN_TEST_H_

#include "main.h"


#include <string.h>
#include <stdio.h>

CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

char msg[100];

UART_HandleTypeDef huart2;

int CAN_Test_Init(CAN_HandleTypeDef hcan, UART_HandleTypeDef huart, uint32_t can_id);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void CAN_Test_SendMessage(CAN_HandleTypeDef *hcan);


#endif /* CAN_TEST_H_ */
