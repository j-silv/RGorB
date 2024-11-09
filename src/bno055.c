/*
 * bno055.c
 *
 *  Created on: Sep 20, 2024
 *      Author: mbac
 */

#include "bno055.h"


uint8_t current_page = PAGE_0;   // keep track of current page
uint8_t interrupt_flags = 0x00;  // COR Shadow reg of int status


struct Sensor_Config
{
	uint8_t data_unit;     // used for converting units
	uint8_t data_range;
};

struct Sensor_Config acc_sensor_config;
struct Sensor_Config mag_sensor_config;
struct Sensor_Config gyr_sensor_config;
struct Sensor_Config eul_sensor_config;
struct Sensor_Config qua_sensor_config;
struct Sensor_Config lia_sensor_config;
struct Sensor_Config grv_sensor_config;
struct Sensor_Config temp_sensor_config;


// If current page different than page register is on, change page
void handle_page(uint8_t page) {
	if (page != current_page) {
		I2C_WRITE(DEV_ID, PAGE_ID_OFFSET, &page);
		current_page = page;
	}
}


uint8_t init_bno055() {
	if (get_dev_id() != CHIP_ID_VALUE) {
		return SYSTEM_I2C_ERROR;
	}
	if (get_acc_id() != ACC_ID_VALUE) {
		return SYSTEM_I2C_ERROR;
	}
	if (get_mag_id() != MAG_ID_VALUE) {
		return SYSTEM_I2C_ERROR;
	}
	if (get_gyro_id() != GYRO_ID_VALUE) {
		return SYSTEM_I2C_ERROR;
	}

	if (get_post_status() == SYSTEM_POST_ERROR) {
		return SYSTEM_POST_ERROR;
	}

	if (run_bist() == BIST_ERROR) {
		return SYSTEM_BIST_ERROR;
	}

	set_power_mode(NORMAL_POWER_MODE);
	set_operation_mode(CONFIG_OP_MODE);


	configure_units(UNIT_WINDOWS,
					UNIT_FAHRENHEIT,
					UNIT_DEGREES,
					UNIT_DPS,
					UNIT_MPS
			);

	configure_temp_source(SOURCE_ACC);

	enable_interrupts(ALL_IE);

	set_operation_mode(NDOF_OP_MODE);

	return NO_SYSTEM_ERROR;
}
//============================================


//============================================
// ID
//============================================
uint8_t get_dev_id() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, CHIP_ID_OFFSET, &rdata);
	return rdata;
}

uint8_t get_acc_id() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, ACC_ID_OFFSET, &rdata);
	return rdata;
}

uint8_t get_mag_id() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, MAG_ID_OFFSET, &rdata);
	return rdata;
}

uint8_t get_gyro_id() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GYR_ID_OFFSET, &rdata);
	return rdata;
}
//============================================


//============================================
// CONFIG
//============================================
void configure_acc(uint8_t g_range, uint8_t bandwidth, uint8_t op_mode){
	uint8_t wdata;
	handle_page(PAGE_1);

	wdata = (g_range | bandwidth | op_mode);
	I2C_WRITE(DEV_ID, ACC_CONFIG_OFFSET, &wdata);
}
//============================================


//============================================
// MODES
//============================================
void set_power_mode(uint8_t power_mode) {
	handle_page(PAGE_0);
	I2C_WRITE(DEV_ID, PWR_MODE_OFFSET, &power_mode);
}

void set_operation_mode(uint8_t operation_mode) {
	handle_page(PAGE_0);
	I2C_WRITE(DEV_ID, OPR_MODE_OFFSET, &operation_mode);

	if (operation_mode == CONFIG_OP_MODE) {
//		TODO: wait 19 ms;
	} else {
//		TODO: wait 7ms);
	}
}
//============================================


//============================================
// ACCELERATION DATA
//============================================
uint16_t get_acc_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, ACC_DATA_X_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, ACC_DATA_X_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_acc_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, ACC_DATA_Y_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, ACC_DATA_Y_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_acc_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, ACC_DATA_Z_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, ACC_DATA_Z_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_acc_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*x_data = get_acc_x_data();
	*y_data = get_acc_y_data();
	*z_data = get_acc_z_data();
}
//============================================


//============================================
// MAGNETOMETER DATA
//============================================
uint16_t get_mag_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, MAG_DATA_X_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, MAG_DATA_X_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_mag_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, MAG_DATA_Y_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, MAG_DATA_Y_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}
uint16_t get_mag_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, MAG_DATA_Z_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, MAG_DATA_Z_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_mag_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*x_data = get_mag_x_data();
	*y_data = get_mag_y_data();
	*z_data = get_mag_z_data();
}
//============================================


//============================================
// MAGNETOMETER DATA
//============================================
uint16_t get_gyro_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GYR_DATA_X_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, GYR_DATA_X_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_gyro_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GYR_DATA_Y_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, GYR_DATA_Y_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_gyro_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GYR_DATA_Z_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, GYR_DATA_Z_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_gyro_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*x_data = get_gyro_x_data();
	*y_data = get_gyro_y_data();
	*z_data = get_gyro_z_data();
}
//============================================


//============================================
// EULER DATA
//============================================
uint16_t get_euler_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, EUL_HEADING_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, EUL_HEADING_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_euler_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, EUL_ROLL_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, EUL_ROLL_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_euler_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, EUL_PITCH_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, EUL_PITCH_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_euler_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*x_data = get_euler_x_data();
	*y_data = get_euler_y_data();
	*z_data = get_euler_z_data();
}
//============================================


//============================================
// QUATERNION DATA
//============================================
uint16_t get_quaternion_w_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, QUA_DATA_W_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, QUA_DATA_W_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_quaternion_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, QUA_DATA_X_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, QUA_DATA_X_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_quaternion_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, QUA_DATA_Y_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, QUA_DATA_Y_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_quaternion_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, QUA_DATA_Z_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, QUA_DATA_Z_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_quaternion_data(uint16_t* w_data, uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*w_data = get_quaternion_w_data();
	*x_data = get_quaternion_x_data();
	*y_data = get_quaternion_y_data();
	*z_data = get_quaternion_z_data();
}
//============================================


//============================================
// LINEAR ACCELERATION DATA
//============================================
uint16_t get_linear_acc_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, LIA_DATA_X_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, LIA_DATA_X_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_linear_acc_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, LIA_DATA_Y_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, LIA_DATA_Y_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_linear_acc_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, LIA_DATA_Z_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, LIA_DATA_Z_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_linear_acc_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*x_data = get_linear_acc_x_data();
	*y_data = get_linear_acc_y_data();
	*z_data = get_linear_acc_z_data();
}
//============================================


//============================================
// GRAVITY VECTOR DATA
//============================================
uint16_t get_gravity_x_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GRV_DATA_X_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, GRV_DATA_X_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_gravity_y_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GRV_DATA_Y_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, GRV_DATA_Y_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

uint16_t get_gravity_z_data() {
	uint8_t rdata_lsb;
	uint8_t rdata_msb;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, GRV_DATA_Z_LSB_OFFSET, &rdata_lsb);
	I2C_READ(DEV_ID, GRV_DATA_Z_MSB_OFFSET, &rdata_msb);

	return (rdata_msb << 8 | rdata_lsb);
}

void get_gravity_data(uint16_t* x_data, uint16_t* y_data, uint16_t* z_data) {
	*x_data = get_gravity_x_data();
	*y_data = get_gravity_y_data();
	*z_data = get_gravity_z_data();
}
//============================================


uint8_t get_temp() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, TEMP_OFFSET, &rdata);

	return rdata;
}



//============================================
// CALIBRATION STATUS
//============================================
void get_calibration_status(uint8_t* sys_cal, uint8_t* gyr_cal, uint8_t* acc_cal, uint8_t* mag_cal) {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, CALIB_STAT_OFFSET, &rdata);

	*sys_cal = (rdata >> 6) & 0x03;
	*gyr_cal = (rdata >> 4) & 0x03;
	*acc_cal = (rdata >> 2) & 0x03;
	*mag_cal = (rdata >> 0) & 0x03;
}
//============================================


//============================================
// SYSTEM ERRORS/STATUS
//============================================
uint8_t get_system_errors() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, SYS_ERR_OFFSET, &rdata);

	return rdata;
}

uint8_t get_system_status() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, SYS_STATUS_OFFSET, &rdata);

	return rdata;
}
//============================================


//============================================
// POST
//============================================
uint8_t get_post_status() {
	uint8_t mcu_st;
	uint8_t gyr_st;
	uint8_t acc_st;
	uint8_t mag_st;

	get_post_results(&mcu_st, &gyr_st, &mag_st, &acc_st);

	if ((mcu_st == 0x00) | (gyr_st == 0x00) | (acc_st == 0x00) | (mag_st == 0x00)) {
		return SYSTEM_POST_ERROR;
	}
	return NO_SYSTEM_ERROR;
}

void get_post_results(uint8_t* mcu_st, uint8_t* gyr_st, uint8_t* mag_st, uint8_t* acc_st) {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, ST_RESULT_OFFSET, &rdata);

	*mcu_st = (rdata >> 3) & 0x01;
	*gyr_st = (rdata >> 2) & 0x01;
	*acc_st = (rdata >> 1) & 0x01;
	*mag_st = (rdata >> 0) & 0x01;
}
//============================================


//============================================
// BIST
//============================================
uint8_t get_bist_status() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, ST_RESULT_OFFSET, &rdata);

	return rdata;
}

uint8_t run_bist() {
	uint8_t rdata;
	uint8_t wdata = TRIGGER_BIST;
	handle_page(PAGE_0);
	I2C_WRITE(DEV_ID, SYS_TRIGGER_OFFSET, &wdata);  // trigger BIST

	// TODO: wait 400ms

	I2C_READ(DEV_ID, SYS_ERR_OFFSET, &rdata);             // get BIST overall status

	if ((rdata & BIST_ERROR) == BIST_ERROR) {
		// BIST FAILED
		return BIST_ERROR;
	} else {
		return NO_ERROR;
	}
}
//============================================


//============================================
// UNITS
//============================================
void configure_units(uint8_t ori_unit, uint8_t temp_unit, uint8_t eul_unit, uint8_t gyro_unit, uint8_t acc_unit) {
	uint8_t wdata;
	handle_page(PAGE_0);

	wdata = (ori_unit << 7) | (temp_unit << 4) | (eul_unit << 2) | (gyro_unit << 1) | (acc_unit << 0);

	I2C_WRITE(DEV_ID, UNIT_SEL_OFFSET, &wdata);
    acc_sensor_config.data_unit = acc_unit;
    mag_sensor_config.data_unit = 0x00;
	gyr_sensor_config.data_unit = gyro_unit;
	eul_sensor_config.data_unit = eul_unit;
	qua_sensor_config.data_unit = 0x00;
	lia_sensor_config.data_unit = acc_unit;
	grv_sensor_config.data_unit = acc_unit;
	temp_sensor_config.data_unit = temp_unit;
}
//============================================


//============================================
// TEMP SOURCE
//============================================
void configure_temp_source(uint8_t temp_source) {
	handle_page(PAGE_0);
	I2C_WRITE(DEV_ID, TEMP_SOURCE_OFFSET, &temp_source);
}
//============================================


//============================================
// INTERRUPTS
//============================================
void enable_interrupts(uint8_t int_mask) {
	handle_page(PAGE_1);
	I2C_WRITE(DEV_ID, INT_EN_OFFSET, &int_mask);
}

void read_interrupt_status_register() {
	uint8_t rdata;
	handle_page(PAGE_0);
	I2C_READ(DEV_ID, INT_STA_OFFSET, &rdata);

	interrupt_flags |= rdata;
}


void reset_interrupt_status() {
	uint8_t wdata = RESET_INT;

	handle_page(PAGE_0);
	I2C_WRITE(DEV_ID, SYS_TRIGGER_OFFSET, &wdata);
}

uint8_t get_interrupt_status(uint8_t int_mask) {
	uint8_t masked_int_data;
	masked_int_data = interrupt_flags & int_mask;  // get selected interrupt value
	interrupt_flags &= !(int_mask);                // clear selected interrupt value

	return masked_int_data;
}

void clear_all_interrupts() {
	read_interrupt_status_register();  // clear device ISR
	reset_interrupt_status();          // reset int output
	get_interrupt_status(ALL_IS);      // clear global variable
}
//============================================


//============================================
// DATA UNIT CONVERSIONS
//============================================
double convert_acc(uint16_t acc_data) {
	if (acc_sensor_config.data_unit == UNIT_MPS) {
		return (short)acc_data / 100.0;   // 100 LSB = 1 m/s^2
	}
	return (short)acc_data;               // 1 LSB = 1 mg
}

double convert_mag(uint16_t mag_data) {
	return (short)mag_data / 16.0;        // 1 LSB = 16 uT
}

double convert_gyro(uint16_t gyro_data) {
	if (gyr_sensor_config.data_unit == UNIT_DPS) {
		return (short)gyro_data / 16.0;   // 1 LSB = 16 DPS
	}
	return (short)gyro_data / 900.0;      // 1 LSB = 900 RPS
}

double convert_euler(uint16_t euler_data) {
	if (eul_sensor_config.data_unit == UNIT_DPS) {
		return (short)euler_data / 16.0;  // 1 LSB = 16 degrees
	}
	return (short)euler_data / 900.0;     // 1 LSB = 900 radians
}

double convert_lin(uint16_t lia_data) {
	if (lia_sensor_config.data_unit == UNIT_MPS) {
		return (short)lia_data / 100.0;   // 100 LSB = 1 m/s^2
	}
	return (short)lia_data;               // 1 LSB = 1 mg
}

double convert_grv(uint16_t grv_data) {
	if (grv_sensor_config.data_unit == UNIT_MPS) {
		return (short)grv_data / 100.0;   // 100 LSB = 1 m/s^2
	}
	return (short)grv_data;               // 1 LSB = 1 mg
}

int convert_temp(uint8_t temp_data) {
	if (temp_sensor_config.data_unit == UNIT_CELSIUS) {
		return temp_data;                 // 1 LSB = 1 degC
	}
	return temp_data * 2;                 // 1 LSB = 2 degF
}
//============================================

