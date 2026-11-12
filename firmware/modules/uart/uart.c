#include "uart.h"
#include "stdint.h"
#include "string.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>



bool print_uart(UART_HandleTypeDef *uart, const char *msg){
    HAL_UART_Transmit(&uart, (uint8_t *) msg, sizeof(msg), HAL_MAX_DELAY);
    return true;
}

void print_uint(UART_HandleTypeDef *uart, uint16_t num){
    char msg[32];
    snprintf(msg, sizeof(msg), "%u\r\n", num);
    print_uart(&uart, msg);
}

void print_float(UART_HandleTypeDef *uart, float num){
  char msg[32];
  snprintf(msg, sizeof(msg), "%.2f\r\n", num);
  print_uart(&uart, msg);
}
