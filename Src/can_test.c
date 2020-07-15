/*
 * can_test.c
 *
 *  Created on: 15Jul.,2020
 *      Author: micha
 */

#include "can_test.h"

int CAN_Test_Init(CAN_HandleTypeDef hcan, UART_HandleTypeDef huart, uint32_t can_id)
{
    // point to initialised huart2
    huart2 = huart;

    TxHeader.ExtId = can_id;    // Maximum 2^29=536870911 0x1FFFFFFF
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_EXT;  // Use extended id
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;
    TxData[0] = 0x00;
    TxData[1] = 0x01;
    TxData[2] = 0x02;
    TxData[3] = 0x03;
    TxData[4] = 0x04;
    TxData[5] = 0x05;
    TxData[6] = 0x06;
    TxData[7] = 0x07;

    CAN_FilterTypeDef  sFilterConfig;

    /*##-2- Configure the CAN Filter ###########################################*/
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
        return -1;
    if (HAL_CAN_Start(&hcan) != HAL_OK)
        return -2;
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        return -3;

    snprintf(msg, sizeof(msg)-1, "CAN_Test\r\nInitialisation successful!\r\n\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    return 0;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
        Error_Handler();
    }
    else
    {
        // Print ID and first data byte to serial
        if (RxHeader.IDE == CAN_ID_STD)
            snprintf(msg, sizeof(msg)-1, "Received SID: 0x%lX  Data: 0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X\r\n",
                    RxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
        else
            snprintf(msg, sizeof(msg)-1, "Received EID: 0x%lX  Data: 0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X\r\n",
                    RxHeader.ExtId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
        if (HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK);
    }
}

void CAN_Test_SendMessage(CAN_HandleTypeDef *hcan)
{
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
    {
        if (HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
            Error_Handler();
        else
        {
            snprintf(msg, sizeof(msg)-1, "Sent! EID: 0x%lX  Data: 0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X  0x%02X\r\n",
                    TxHeader.ExtId, TxData[0]++, TxData[1], TxData[2], TxData[3], TxData[4], TxData[5], TxData[6], TxData[7]);
            if (HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK);
        }
    }
    HAL_Delay(100);
}
