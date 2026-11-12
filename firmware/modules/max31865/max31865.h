/* 
    Header of the MAX31865 library
    
    Because the cloned version does not have the same value of the R_ref for
    all the sensors --> has to take into account the value of R_ref, (standard R_ref value is 431 ohm)
*/
#ifndef MAX31865_H
#define MAX31865_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

typedef struct{
    // SPI bus
    SPI_HandleTypeDef *hspi;
    // port of the CS for example GPIOA
    GPIO_TypeDef *cs_port;
    // Pin
    uint16_t cs_pin_name;
    // three wire
    bool three_wire;
    float r_ref;
} max31865_t;

// configure one probe, returns true if the board answered correctly
bool max31865_init(const max31865_t *probe); 

uint8_t max31865_read_config(const max31865_t *probe);

// read the 15-bit raw value of one probe, returns false on fault / SPI error
bool max31865_read_raw(const max31865_t *probe, uint16_t *raw);

// convert a 16 bit binary temperature (proportion on the card's r_ref but they are different)
float max31865_convert_celcius(uint16_t temp_bin, float r_ref);

#endif 