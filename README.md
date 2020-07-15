# CAN_Test
Basic CAN send and receive test library.
#### Tested with STM32F334R8 and MCP2562 Transcievers
Press B1 User button (Blue) to send a CAN message.\
Received messages are displayed over uart2 (USB Serial) **38400 8N1**.\
No/very minimal error checking is done. For blocking error checking modify `Error_Handler()` as shown below. This will flash LD2 rapidly on any HAL error, which **WILL** happen when initialising CAN module without a transceiver correctly connected. Press onboard RESET button to restart.
## How to use
* Create new STM32 project, initialise CAN in master mode with Tq = 500ns, BS1 = 13, BS2 = 2, SJW = 1. Enable NVIC interrupt for CAN RX0. This sets CAN bitrate to 125k with 16 BTq, 87.5% sample point.
* Clone/copy repository.
* Add `can_test.c` and `can_test.h` to project directory `/Scr`.
* Either copy (replacing original) `main.c` to `/Scr` directory, or add/modify the following lines to `main.c`.
```
/* USER CODE BEGIN Includes */
#include "can_test.h"
/* USER CODE END Includes */
```
```
/* USER CODE BEGIN 2 */
CAN_Test_Init(hcan, huart2, 0x111);
/* USER CODE END 2 */
```
```
    /* USER CODE BEGIN 3 */
    CAN_Test_SendMessage(&hcan);
}
/* USER CODE END 3 */
```
* Change the default function `Error_Handler()` as below for blocking error checking.
```
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        HAL_Delay(50);
    }
    /* USER CODE END Error_Handler_Debug */
}
```