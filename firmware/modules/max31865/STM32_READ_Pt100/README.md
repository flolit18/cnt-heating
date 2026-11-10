# STM32_READ_Pt100

To read the temperature measured by a Pt100 and print it to a terminal. 3-wire configuration. but I messed up the board so I dont know what to call it
but it has 1 resistor on top.

## Hardware

- **MCU board:** NUCLEO-F446RE (STM32F446RE)
- **RTD converter:** MAX31865, reference resistor Rref = 430 Ω (marked "431")
- **Sensor:** Pt100, 3 wires (two wires of the same color + one different)

## Wiring

| MAX31865 | Function | NUCLEO-F446RE |
|---|---|---|
| CLK | SPI2 SCK | PB10 (D6) |
| SDO | SPI2 MISO | PC2 (morpho CN7) |
| SDI | SPI2 MOSI | PC1 (A4) |
| CS | Chip select (GPIO output, idle high) | PA1 (A1) |
| VIN | Power | 3.3V |
| GND | Ground | GND |

These are my configs, I use GPIO for CS.

## MAX31865 configuration

Configuration register (write address `0x80`):

| Bit | Meaning | Value |
|---|---|---|
| D7 | V_BIAS on | 1 |
| D6 | Auto conversion | 1 |
| D4 | 3-wire (1) / 2- or 4-wire (0) | see note below |
| D1 | Fault status clear (self-clearing) | 1 |
| D0 | 50 Hz filter | 1 |

- **3-wire** (normal case): `0xD3`. Reads back as `0xD1`.
- **2/4-wire** (what this board currently uses): `0xC3`. Reads back as `0xC1`.

> **Note:** for a real 3-wire setup, the board jumpers must be changed: solder **"2/3 Wire"**, cut the trace between the middle pad and **"24"**, and solder the middle pad to **"3"**. On my board the "24" cut was incomplete: the middle pad (FORCE2) stayed connected to GND, which shorted F+ to ground and gave RTD = 0. The "3" side is therefore left unsoldered and the chip runs in 2/4-wire mode (`0xC3`), with "2/3 Wire" soldered. This works, but the resistance of the single wire adds to the measurement (a few °C too high).

## Reading the RTD

Read from address `0x01`, keeping CS low for 3 bytes (`0x01, 0x00, 0x00`). The chip auto-increments to `0x02`:

```
RTD = ((MSB << 8) | LSB) >> 1     // bit 0 = fault flag
R   = RTD * 430 / 32768           // ohms
T   = (R - 100) / 0.385           // °C, linear approximation (~1 °C between 0 and 100 °C)
```
>  **Note:** : should be calibrated and improved later. In fact, I upload this while waiting for the new materials to arrive. I still want to try to resolder the 4-3 with a thin tin bridge, and I hope that should not ground again **F+**.

Fault status register: `0x07` (`0x00` = no fault, `0x40` = RTD below low threshold, `0x80` = above high threshold).

## Lessons learned

- **Enable SPI before the first transfer:** call `__HAL_SPI_ENABLE(&hspi2)` before `config_max31865()`. Otherwise the clock line glitches when HAL enables SPI after CS is already low, and the first write (the configuration) is corrupted.
- **SPI pin speed = Low** (CubeMX → GPIO → SPI). "Very High" causes ringing on jumper wires and corrupted bytes, especially when you are using Aliexpress Dupont wires like me.
