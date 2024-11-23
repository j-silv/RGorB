![Build status](https://github.com/github/docs/actions/workflows/main.yml/badge.svg)

# Overview

**RG**or**B** (*Red Green or Blue*) is an embedded systems project which maps Inertial Measurement Unit (IMU) data to an LED color space. The RGB colors are then displayed on LED strips (NeoPixels) which are enclosed in a 3D-printed translucent sphere (an orb).

The initial inspiration for this project was to convert a rotation angle to RGB values, and thus be able to "rotate" through colors visually by physically manipulating a sphere's position in space. Using additional IMU sensor data, more modes can be envisioned that blend together a shape's physical and visual characteristics.

This repository is a work in progress, and currently houses the software components of **RG**or**B**. 

# Hardware

## Processor

The brains of the system is a microcontroller, which handles sensor data and LED strip communication. The STM32 brand of microcontrollers was chosen due to their low cost, powerful peripheral set, and ability to program in "bare-metal". While an Arduino or some other equivalent ecosystem could have been used, the authors wanted to make the system as extensible as possible from the onset. This also served as a learning opportunity to dive into low-level embedded systems programming.

A NUCLEO-F446RE board from STM32 was primarily used for development purposes. For the final system, however, a much less powerful microcontroller is sufficient. As the orb will be battery powered, something from the STM32L4 family is a reasonable choice (e.g. STM32L422KBT6).

## Sensor

The BNO055 IMU chip was used to receive accelerometer, gyroscope, and magnetometer data. This is a powerful smart sensor which communicates via I2C protocol with the main microcontroller.

Michael Derrenbacher (one of the project authors), gave an [in-depth walkthrough of the device on YouTube](https://www.youtube.com/watch?v=0xjDUMwMOhk).

## LEDs

For displaying the mapped color data, NeoPixel strips were chosen. The WS2811 is typically the driver IC which these strips typically use. WS2812B is a variant of this device which was used for development purposes.

# Software

A conscious effort was made to keep software dependencies as minimal as possible. Although STM32 offers abstraction layers such as HAL or LL, these were avoided. Going bare-metal helps to lower memory usage, achieve better performance, and offers transparency into how the hardware operates. Another benefit of this flow is that the entire build/flash/debug process can become independent of any vendor-specific IDE. It can be argued that using HAL (or, once again, an Arduino ecosystem) would of saved a lot of time. However, such a decision is still a tradeoff between performance and code comprehension.



## Directory structure

Several core files are provided by STM32 and are compliant with CMSIS. Custom bare-metal drivers were created by reading the microcontroller's reference manual and datasheet.

```
├── config
│   ├── startup_stm32f446xx.s      --> Assembly startup code provided by STM32  
│   ├── STM32F446RETX_FLASH.ld     --> Linker script provided by STM32
│   └── STM32F446.svd              --> Register and peripheral map for debugging
├── include
│   ├── cmsis/                     --> CMSIS .h header files for Arm Cortex M4   
│   ├── bno055.h                   --> IMU header file
│   ├── clock.h                    --> Clock header file
│   ├── i2c.h                      --> I2C protocol header file
│   ├── stm32f446xx.h              --> Device header file provided by STM32
│   ├── stm32f4xx.h                --> Device header file provided by STM32
│   ├── system_stm32f4xx.h         --> Device header file provided by STM32
│   └── ws2812b.h                  --> LED strip header file
├── src
│   ├── bno055.c                   --> IMU driver
│   ├── clock.c                    --> Clock driver (configures STM32 clock)
│   ├── i2c.c                      --> I2C peripheral driver
│   ├── main.c                     --> Main application entry point
│   ├── system_stm32f4xx.c         --> SystemCoreClock provided by STM32
│   └── ws2812b.c                  --> LED strip driver
├── LICENSE.md                     --> Project license 
├── Makefile                       --> Builds and flashes firmware using Make         
└── README.md                      --> This README file

```

## How to run

To build firmware for the STM32, you need a build system, a compiler toolchain and a device programmer. This setup was inspired from the following tutorials:

- [A bare metal programming guide](https://github.com/cpq/bare-metal-programming-guide)
- ["Bare Metal" STM32 Programming ](https://vivonomicon.com/2018/04/02/bare-metal-stm32-programming-part-1-hello-arm/)

The following steps were tested on an Ubuntu 22.04 OS (Linux).

### 1. Dependencies

First, download the meta-package which contains tools for typical software development on Linux.

```
sudo apt install build-essential
```

For Ubuntu 22.04 and later, you might need to run the following command. The Linux distribution upgrades to `libncurses6` but the ARM toolchain still hasn't.

```
sudo apt-get install libncursesw5
```

Next, you need to download the ARM compiler toolchain. You can follow [the steps outlined here](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).

Finally, you need a way to actually program the microcontroller once the firmware is built. STM32 offers [an open-source toolset](https://github.com/stlink-org/stlink) which works great with the STLINK programmer boards.

Optionally, you can also use an IDE for debugging and ease of source code development. For this project, Visual Studio (VS) Code, a powerful and open-source software development environment, was leveraged with the [Cortex Debug extension](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug). 

### 2. Build and flash software

Once the above dependencies are installed, you should be able to clone this repository and then run `make` to build, link, and flash the software to an attached STM32F446RE device. The generated files from this process are stored in the `build/` folder.

```
git clone https://github.com/j-silv/RGorB.git
cd RGorB/
make
```

You can also run the build and compilation steps separately:

```
make build   # creates object files from source code
make link    # links the object files together into a single executable
make flash   # flashes the firmware image to the microcontroller
```

### 3. Debug

While not required, stepping through the code and analyzing register statuses can be very useful in embedded systems development. The project authors used the open-source VS code environment and the [Cortex Debug extension](https://github.com/Marus/cortex-debug) with custom `launch.json` and `tasks.json` files to achieve this.

Another option is to use the debugger provided by the ARM toolchain via the command line

```
arm-none-eabi-gdb ./build/firmware.elf
```
