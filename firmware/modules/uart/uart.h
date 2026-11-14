#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"




// affiche a message to terminal 
bool print_uart(UART_HandleTypeDef *uart, const char *msg);

// forme a message to parse to print_uart (int 16 bit)
void print_uint(UART_HandleTypeDef *uart, uint16_t num);

// forme a message to parse to print_uart (float)
void print_float(UART_HandleTypeDef *uart, float num);




#endif