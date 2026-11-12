#include "max31865.h"
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "string.h"
#include <math.h>
/*
    R(T) = R0*(1 + alpha * T + beta * T^2)
    alpha = 3.90830e-3
    beta = -5.77500e-3
*/
static void cs_low(const max31865_t *probe){
    HAL_GPIO_WritePin(probe->cs_port, probe->cs_pin_name, GPIO_PIN_RESET);
}

static void cs_high(const max31865_t *probe){
    HAL_GPIO_WritePin(probe->cs_port, probe->cs_pin_name, GPIO_PIN_SET);
}

static void write_reg(const max31865_t *probe, uint8_t reg, uint8_t value){
    uint8_t tx[2] = {reg | 0x80, value};
    HAL_Delay(100);
    cs_low(probe);
    HAL_SPI_Transmit(probe->hspi, tx, sizeof(tx), 20);
    cs_high(probe);
    HAL_Delay(200);
}

static uint8_t read_reg(const max31865_t *probe, uint8_t reg){
    uint8_t tx[2] = {
        reg, 0x00
    };
    uint8_t rx[2];
    cs_low(probe);
    HAL_SPI_TransmitReceive(probe->hspi, tx, rx, sizeof(rx), 20);
    cs_high(probe);
    return rx[1];
}

bool max31865_init(const max31865_t *probe){
    /*
        Choose the config : 0xD1 for 3 wires ifnot 0xC1
        Write in the register 0x00
        Reread the register 0x00
        Return true if the config is what we want
    */
    uint8_t config = probe->three_wire ? 0xD1 : 0xC1;
    write_reg(probe, 0x00, config | 0x02);
    return read_reg(probe, 0x00) == config;
}

uint8_t max31865_read_config(const max31865_t *probe){
    return read_reg(probe, 0x00);
}

bool max31865_read_raw(const max31865_t *probe, uint16_t *raw){
    /*
        read the MSB and LSB 0x01 and 0x02
        if the LSB of the LSB word is 1, then it is a fault --> return false
        if not shift left and return true
    */
    uint8_t msb = read_reg(probe, 0x01);
    uint8_t lsb = read_reg(probe, 0x02);

    uint16_t value = ((uint16_t)msb << 8) | lsb;
    if (value & 0x0001) {  
        return false;
    }
    *raw = value >> 1;
    return true;
}

float max31865_convert_celcius(uint16_t temp_bin, float r_ref){

    const float A = 3.9083e-3f;
    const float B = -5.775e-7f;
    
    float r_t   = (float)temp_bin * r_ref / 32768.0f;   
    float ratio = r_t / 100.0f;                    

    return (-A + sqrtf(A*A - 4.0f*B*(1.0f - ratio))) / (2.0f*B); 
}
