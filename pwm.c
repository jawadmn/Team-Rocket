/**************************************\
 * File Name:      pwm.c
 * Project Name:   EECS373 Final Project
 * Created by:     Jawad Nasser
 * Modified by:    Adrian Padin
 * 			       Emily Rowland
 * 				   Ben Miron
 * Start date:     23 March 2016
 * Last modified:  24 March 2016
 \**************************************/

#include "pwm.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


// Set the duty cycle to a value between 0 and 100
void setPWMDutyRight(uint32_t duty) {

	// Duty should be between 0 and 100
	printf("setPWMDutyRight: %u\r\n", duty);
	assert(duty >= 0 && duty <= 100);

	// Write to offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;

	// Mask the current PWM vals from the other half and set the current duty input
	uint32_t targetVal = *addr_ptr;
	targetVal &= PWM_RIGHT_MASK;
	targetVal |= duty;

	*addr_ptr = targetVal;
}

// Set the duty cycle to a value between 0 and 100
void setPWMDutyLeft(uint32_t duty) {

	// Duty should be between 0 and 100
	printf("setPWMDutyLeft: %u\r\n", duty);
	assert(duty >= 0 && duty <= 100);

	// Write to offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	// Mask the current PWM vals from the other half and set the current duty input
	uint32_t targetVal = *addr_ptr;
	targetVal &= PWM_LEFT_MASK;
	targetVal |= (duty << 8);

	*addr_ptr = targetVal;
}

// Get the duty cycle
uint32_t getPWMDuty(void) {

	// Read from offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	return *addr_ptr;
}

// Set the H-Bridge bits
void setHBridgeInputs(uint32_t H_in) {

	// These are invalid values and will harm the H-bridge
	assert(!((H_in & H_BRIDGE_RIGHT_MASK) == 0x3));
	assert(!((H_in & H_BRIDGE_LEFT_MASK) == 0xC));

	// First read value to do error checking, offset 4
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	addr_ptr += 1;

	// Write to offset 4
	// Write 0 first for safety with h-bridge
	*addr_ptr = 0;
	*addr_ptr = H_in;
}


// Get the duty cycle
uint32_t getHBridgeInputs(void) {

	// Read from offset 4
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	++addr_ptr;
	return *addr_ptr;
}


//Calculates the duty cycle
uint32_t calc_duty(uint8_t input){
	if(input < 128){
		return (uint32_t)floor((double)((128 - input) / 1.27));
	}

	else if(input > 128){
		return (uint32_t)floor((double)((input - 128) / 1.27));
	}

	else{
		return 0;
	}
}


//Calculates the H Bridge Inputs for the right side of the car
uint32_t calc_HBridgeRight(uint8_t input){

	uint32_t current_H = H_BRIDGE_RIGHT_MASK & getHBridgeInputs();

	if(input > 128){
		return current_H + 0x1;
	}

	else if(input < 128){
		return current_H + 0x2;
	}

	else{
		return current_H;
	}
}


//Calculates the H Bridge Inputs for the left side of the car
uint32_t calc_HBridgeLeft(uint8_t input){

	uint32_t current_H = H_BRIDGE_LEFT_MASK & getHBridgeInputs();

	if(input > 128){
		return current_H + 0x8;
	}

	else if(input < 128){
		return current_H + 0x4;
	}

	else{
		return current_H;
	}
}
