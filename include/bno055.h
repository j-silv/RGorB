/*
 * bno055.h
 *
 *  Created on: Sep 20, 2024
 *      Author: mbac
 */

#ifndef BNO055_H_
#define BNO055_H_

#include <stdint.h>
#include "i2c.h"

//============================================
// PAGE 0 REGISTERS
//============================================
#define CHIP_ID_OFFSET           0x00
#define ACC_ID_OFFSET            0x01
#define MAG_ID_OFFSET            0x02
#define GYR_ID_OFFSET            0x03
#define SW_REV_ID_LSB_OFFSET     0x04
#define SW_REV_ID_MSB_OFFSET     0x05
#define BL_REV_OFFSET            0x06
#define PAGE_ID_OFFSET           0x07
#define ACC_DATA_X_LSB_OFFSET    0x08
#define ACC_DATA_X_MSB_OFFSET    0x09
#define ACC_DATA_Y_LSB_OFFSET    0x0A
#define ACC_DATA_Y_MSB_OFFSET    0x0B
#define ACC_DATA_Z_LSB_OFFSET    0x0C
#define ACC_DATA_Z_MSB_OFFSET    0x0D
#define MAG_DATA_X_LSB_OFFSET    0x0E
#define MAG_DATA_X_MSB_OFFSET    0x0F
#define MAG_DATA_Y_LSB_OFFSET    0x10
#define MAG_DATA_Y_MSB_OFFSET    0x11
#define MAG_DATA_Z_LSB_OFFSET    0x12
#define MAG_DATA_Z_MSB_OFFSET    0x13
#define GYR_DATA_X_LSB_OFFSET    0x14
#define GYR_DATA_X_MSB_OFFSET    0x15
#define GYR_DATA_Y_LSB_OFFSET    0x16
#define GYR_DATA_Y_MSB_OFFSET    0x17
#define GYR_DATA_Z_LSB_OFFSET    0x18
#define GYR_DATA_Z_MSB_OFFSET    0x19
#define EUL_HEADING_LSB_OFFSET   0x1A
#define EUL_HEADING_MSB_OFFSET   0x1B
#define EUL_ROLL_LSB_OFFSET      0x1C
#define EUL_ROLL_MSB_OFFSET      0x1D
#define EUL_PITCH_LSB_OFFSET     0x1E
#define EUL_PITCH_MSB_OFFSET     0x1F

#define QUA_DATA_W_LSB_OFFSET    0x20
#define QUA_DATA_W_MSB_OFFSET    0x21
#define QUA_DATA_X_LSB_OFFSET    0x22
#define QUA_DATA_X_MSB_OFFSET    0x23
#define QUA_DATA_Y_LSB_OFFSET    0x24
#define QUA_DATA_Y_MSB_OFFSET    0x25
#define QUA_DATA_Z_LSB_OFFSET    0x26
#define QUA_DATA_Z_MSB_OFFSET    0x27
#define LIA_DATA_X_LSB_OFFSET    0x28
#define LIA_DATA_X_MSB_OFFSET    0x29
#define LIA_DATA_Y_LSB_OFFSET    0x2A
#define LIA_DATA_Y_MSB_OFFSET    0x2B
#define LIA_DATA_Z_LSB_OFFSET    0x2C
#define LIA_DATA_Z_MSB_OFFSET    0x2D
#define GRV_DATA_X_LSB_OFFSET    0x2E
#define GRV_DATA_X_MSB_OFFSET    0x2F
#define GRV_DATA_Y_LSB_OFFSET    0x30
#define GRV_DATA_Y_MSB_OFFSET    0x31
#define GRV_DATA_Z_LSB_OFFSET    0x32
#define GRV_DATA_Z_MSB_OFFSET    0x33
#define TEMP_OFFSET              0x34
#define CALIB_STAT_OFFSET        0x35
#define ST_RESULT_OFFSET         0x36
#define INT_STA_OFFSET           0x37
#define SYS_CLK_STA_TUS_OFFSET   0x38
#define SYS_STATUS_OFFSET        0x39
#define SYS_ERR_OFFSET           0x3A
#define UNIT_SEL_OFFSET          0x3B
//#define RESERVED_OFFSET   0x3C
#define OPR_MODE_OFFSET          0x3D
#define PWR_MODE_OFFSET          0x3E
#define SYS_TRIGGER_OFFSET       0x3F
#define TEMP_SOURCE_OFFSET       0x40
#define AXIS_MAP_CONFIG_OFFSET   0x41
#define AXIS_MAP_SIGN_OFFSET     0x42
#define SIC_MATRIX_LSB0_OFFSET   0x43
#define SIC_MATRIX_MSB0_OFFSET   0x44
#define SIC_MATRIX_LSB1_OFFSET   0x45
#define SIC_MATRIX_MSB1_OFFSET   0x46
#define SIC_MATRIX_LSB2_OFFSET   0x47
#define SIC_MATRIX_MSB2_OFFSET   0x48
#define SIC_MATRIX_LSB3_OFFSET   0x49
#define SIC_MATRIX_MSB3_OFFSET   0x4A
#define SIC_MATRIX_LSB4_OFFSET   0x4B
#define SIC_MATRIX_MSB4_OFFSET   0x4C
#define SIC_MATRIX_LSB5_OFFSET   0x4D
#define SIC_MATRIX_MSB5_OFFSET   0x4E
#define SIC_MATRIX_LSB6_OFFSET   0x4F
#define SIC_MATRIX_MSB6_OFFSET   0x50
#define SIC_MATRIX_LSB7_OFFSET   0x51
#define SIC_MATRIX_MSB7_OFFSET   0x52
#define SIC_MATRIX_LSB8_OFFSET   0x53
#define SIC_MATRIX_MSB8_OFFSET   0x54
#define ACC_OFFSET_X_LSB_OFFSET  0x55
#define ACC_OFFSET_X_MSB_OFFSET  0x56
#define ACC_OFFSET_Y_LSB_OFFSET  0x57
#define ACC_OFFSET_Y_MSB_OFFSET  0x58
#define ACC_OFFSET_Z_LSB_OFFSET  0x59
#define ACC_OFFSET_Z_MSB_OFFSET  0x5A
#define MAG_OFFSET_X_LSB_OFFSET  0x5B
#define MAG_OFFSET_X_MSB_OFFSET  0x5C
#define MAG_OFFSET_Y_LSB_OFFSET  0x5D
#define MAG_OFFSET_Y_MSB_OFFSET  0x5E
#define MAG_OFFSET_Z_LSB_OFFSET  0x5F
#define MAG_OFFSET_Z_MSB_OFFSET  0x60
#define GYR_OFFSET_X_LSB_OFFSET  0x61
#define GYR_OFFSET_X_MSB_OFFSET  0x62
#define GYR_OFFSET_Y_LSB_OFFSET  0x63
#define GYR_OFFSET_Y_MSB_OFFSET  0x64
#define GYR_OFFSET_Z_LSB_OFFSET  0x65
#define GYR_OFFSET_Z_MSB_OFFSET  0x66
#define ACC_RADIUS_LSB_OFFSET    0x67
#define ACC_RADIUS_MSB_OFFSET    0x68
#define MAG_RADIUS_LSB_OFFSET    0x69
#define MAG_RADIUS_MSB_OFFSET    0x6A
//============================================


//============================================
// PAGE 1 REGISTERS
//============================================
#define ACC_CONFIG_OFFSET          0x08
#define MAG_CONFIG_OFFSET          0x09
#define GYR_CONFIG_0_OFFSET        0x0A
#define GYR_CONFIG_1_OFFSET        0x0B
#define ACC_SLEEP_CONFIG_1_OFFSET  0x0C
#define GYR_SLEEP_CONFIG_OFFSET    0x0D
//#define RESERVED_OFFSET  0x0E
#define INT_MSK_OFFSET             0x0F
#define INT_EN_OFFSET              0x10
#define ACC_AM_THRES_OFFSET        0x11
#define ACC_INT_SETTINGS_OFFSET    0x12
#define ACC_HG_DURATION_OFFSET     0x13
#define ACC_HG_THRES_OFFSET        0x14
#define ACC_NM_THRES_OFFSET        0x15
#define ACC_NM_SET_OFFSET          0x16
#define GYR_INT_SETTINGS_OFFSET    0x17
#define GYR_HR_X_SET_OFFSET        0x18
#define GYR_DUR_X_OFFSET           0x19
#define GYR_HR_Y_SET_OFFSET        0x1A
#define GYR_DUR_Y_OFFSET           0x1B
#define GYR_HR_Z_SET_OFFSET        0x1C
#define GYR_DUR_Z_OFFSET           0x1D
#define GYR_AM_THRES_OFFSET        0x1E
#define GYR_AM_SET_OFFSET          0x1F
//============================================


//============================================
// UNITS
//============================================
#define UNIT_WINDOWS  0x00
#define UNIT_ANDROID  0x01
#define UNIT_CELSIUS     0x00
#define UNIT_FAHRENHEIT  0x01
#define UNIT_DEGREES  0x00
#define UNIT_RADIANS  0x01
#define UNIT_DPS  0x00
#define UNIT_RPS  0x01
#define UNIT_MPS  0x00
#define UNIT_MG   0x01
//============================================


//============================================
// TEMPERATURE SOURCE
//============================================
#define SOURCE_ACC   0x00
#define SOURCE_GYRO  0x01
//============================================


//============================================
// CONSTANTS
//============================================
#define DEV_ID  0x28<<1
#define PAGE_0  0x00
#define PAGE_1  0x01
//============================================


//============================================
// CONSTANTS
//============================================
#define CHIP_ID_VALUE  0xA0
#define ACC_ID_VALUE   0xFB
#define MAG_ID_VALUE   0x32
#define GYRO_ID_VALUE  0x0F
//============================================


//============================================
// ERROR STATUSES
//============================================
#define NO_SYSTEM_ERROR       0x00
#define SYSTEM_I2C_ERROR      0x01
#define SYSTEM_POST_ERROR     0x02
#define SYSTEM_BIST_ERROR     0x03
//============================================


//============================================
// SYSTEM ERROR STATUSES
//============================================
#define NO_ERROR               0x00
#define PERIPHERAL_INIT_ERROR  0x01
#define SYS_INIT_POST_ERROR    0x02
#define BIST_ERROR             0x03
#define VALUE_RANGE_ERROR      0x04
#define ADDRESS_RANGE_ERROR    0x05
#define WRITE_ERROR            0x06
#define LOW_POWER_OP_ERROR     0x07
#define ACC_POWER_ERROR        0x08
#define FUSTION_CONFIG_ERROR   0x09
//============================================


//============================================
// SYSTEM STATUSES
//============================================
#define IDLE_STATUS               0x00
#define SYSTEM_ERROR_STATUS       0x01
#define PERIPHERAL_INIT_STATUS    0x02
#define SYS_INIT_STATUS           0x03
#define POST_STATUS               0x04
#define FUSION_RUNNING_STATUS     0x05
#define NO_FUSION_RUNNING_STATUS  0x06
//============================================


//============================================
// POWER MODES
//============================================
#define NORMAL_POWER_MODE   0x00
#define LOW_POWER_MODE      0x01
#define SUSPEND_POWER_MODE  0x02
//============================================


//============================================
// OPERATING MODES
//============================================
#define CONFIG_OP_MODE        0x00
#define ACC_ONLY_OP_MODE      0x01
#define MAG_ONLY_OP_MODE      0x02
#define GYRO_ONLY_OP_MODE     0x03
#define ACC_MAG_OP_MODE       0x04
#define ACC_GYRO_OP_MODE      0x05
#define MAG_GYRO_OP_MODE      0x06
#define AMG_OP_MODE           0x07
#define IMU_OP_MODE           0x08
#define COMPASS_OP_MODE       0x09
#define M4G_OP_MODE           0x0A
#define NDOF_FMC_OFF_OP_MODE  0x0B
#define NDOF_OP_MODE          0x0C
//============================================


//============================================
// SYSTEM TRIGGERS
//============================================
#define TRIGGER_BIST  0x01
#define RESET_DEVICE  0x20
#define RESET_INT     0x40
//============================================


//============================================
// ACC CONFIG
//============================================
#define ACC_CONFIG_GRANGE_2G   0x00
#define ACC_CONFIG_GRANGE_4G   0x01
#define ACC_CONFIG_GRANGE_8G   0x02
#define ACC_CONFIG_GRANGE_16G  0x03

#define ACC_CONFIG_BANDWIDTH_1   0x00  //   7.810 Hz
#define ACC_CONFIG_BANDWIDTH_2   0x04  //   15.63 Hz
#define ACC_CONFIG_BANDWIDTH_3   0x08  //   31.25 Hz
#define ACC_CONFIG_BANDWIDTH_4   0x0C  //   62.50 Hz
#define ACC_CONFIG_BANDWIDTH_5   0x10  //  125.00 Hz
#define ACC_CONFIG_BANDWIDTH_6   0x14  //  250.00 Hz
#define ACC_CONFIG_BANDWIDTH_7   0x18  //  500.00 Hz
#define ACC_CONFIG_BANDWIDTH_8   0x1C  // 1000.00 Hz

#define ACC_CONFIG_MODE_NORMAL        0x00
#define ACC_CONFIG_MODE_SUSPEND       0x20
#define ACC_CONFIG_MODE_LOW_POWER_1   0x40
#define ACC_CONFIG_MODE_STANDBY       0x60
#define ACC_CONFIG_MODE_LOW_POWER_2   0x80
#define ACC_CONFIG_MODE_DEEP_SUSPEND  0xA0
//============================================


//============================================
// INTERRUPTS
//============================================
#define ACC_BSX_DRDY_IE   0x01  // Accelerometer or BSX Data Ready
#define MAG_DRDY_IE       0x02  // Magnetometer Data Ready
#define GYRO_AM_IE        0x04  // Gyroscope Any Motion
#define GYR_HIGH_RATE_IE  0x08  // Gyroscope High Rate
#define GYR_DRDY_IE       0x10  // Gyroscope Data Ready
#define ACC_HIGH_G_IE     0x20  // Accelerometer High-G
#define ACC_AM_IE         0x40  // Accelerometer Any Motion
#define ACC_NM_IE         0x80  // Accelerometer No Motion
#define ALL_IE            0xFF  // All Interrupts

#define ACC_BSX_DRDY_IS   0x01  // Accelerometer or BSX Data Ready
#define MAG_DRDY_IS       0x02  // Magnetometer Data Ready
#define GYRO_AM_IS        0x04  // Gyroscope Any Motion
#define GYR_HIGH_RATE_IS  0x08  // Gyroscope High Rate
#define GYR_DRDY_IS       0x10  // Gyroscope Data Ready
#define ACC_HIGH_G_IS     0x20  // Accelerometer High-G
#define ACC_AM_IS         0x40  // Accelerometer Any Motion
#define ACC_NM_IS         0x80  // Accelerometer No Motion
#define ALL_IS            0xFF  // All Interrupts
//============================================


void handle_page(uint8_t page);

uint8_t init_bno055();

uint8_t get_dev_id();
uint8_t get_acc_id();
uint8_t get_mag_id();
uint8_t get_gyro_id();

void configure_acc(uint8_t g_range, uint8_t bandwidth, uint8_t op_mode);

uint16_t get_acc_x_data();
uint16_t get_acc_y_data();
uint16_t get_acc_z_data();
void     get_acc_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint16_t get_mag_x_data();
uint16_t get_mag_y_data();
uint16_t get_mag_z_data();
void     get_mag_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint16_t get_gyro_x_data();
uint16_t get_gyro_y_data();
uint16_t get_gyro_z_data();
void     get_gyro_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint16_t get_euler_x_data();
uint16_t get_euler_y_data();
uint16_t get_euler_z_data();
void     get_euler_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint16_t get_quaternion_w_data();
uint16_t get_quaternion_x_data();
uint16_t get_quaternion_y_data();
uint16_t get_quaternion_z_data();
void     get_quaternion_acc_data(uint16_t* w_data, uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint16_t get_linear_acc_x_data();
uint16_t get_linear_acc_y_data();
uint16_t get_linear_acc_z_data();
void     get_linear_acc_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint16_t get_gravity_x_data();
uint16_t get_gravity_y_data();
uint16_t get_gravity_z_data();
void     get_gravity_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data);

uint8_t get_temp();

void get_calibration_status(uint8_t* sys_cal, uint8_t* gyr_cal, uint8_t* acc_cal, uint8_t* mag_cal);

uint8_t get_post_status();
void get_post_results(uint8_t* mcu_st, uint8_t* gyr_st, uint8_t* mag_st, uint8_t* acc_st);

uint8_t get_bist_status();
uint8_t run_bist();

uint8_t get_system_errors();
uint8_t get_system_status();

void configure_units(uint8_t ori_unit, uint8_t temp_unit, uint8_t eul_unit, uint8_t gyro_unit, uint8_t acc_unit);

void configure_temp_source(uint8_t ori_unit);

void set_power_mode(uint8_t power_mode);

void set_operation_mode(uint8_t operation_mode);

void enable_interrupts(uint8_t int_mask);
void read_interrupt_status_register();
void reset_interrupt_status();
uint8_t get_interrupt_status(uint8_t int_mask);
void clear_all_interrupts();





double convert_acc(uint16_t acc_data);
double convert_mag(uint16_t mag_data);
double convert_gyro(uint16_t gyro_data);
double convert_euler(uint16_t euler_data);
double convert_lin(uint16_t lia_data);
double convert_grv(uint16_t grv_data);
int convert_temp(uint8_t temp_data);


#endif /* BNO055_H_ */
