#include "uart.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include <stdbool.h>



bool print_uart(UART_HandleTypeDef *uart, const char *msg){
    if (uart == NULL || msg == NULL) {
        return false;
    }
    size_t len = strlen(msg);
    if (len == 0) {
        return true;
    }
    return HAL_UART_Transmit(uart, (const uint8_t *)msg, (uint16_t)len, HAL_MAX_DELAY) == HAL_OK;
}

bool print_uint(UART_HandleTypeDef *uart, uint16_t num){
    char msg[32];
    snprintf(msg, strlen(msg), "%u\r\n", (unsigned)num);
    return print_uart(uart, msg);
}

bool print_float(UART_HandleTypeDef *uart, float num){
    char msg[32];
    snprintf(msg, strlen(msg), "%.2f\r\n", (double)num);
    return print_uart(uart, msg);
}
