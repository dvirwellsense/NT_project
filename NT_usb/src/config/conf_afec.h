/*
 * conf_afec.h
 *
 * Created: 1/4/2023 2:28:21 PM
 *  Author: DvirShavit
 */ 

#ifndef CONF_AFEC_H_
#define CONF_AFEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> // Include this for va_list and related functions

/** The maximal digital value */
#define MAX_DIGITAL_12_BIT     (4095UL)

#define ROWS 60
#define COLUMNS 30
#define REF 3
#define MEAS 17
#define LAST 7
#define OPEN 100
#define CLOSE 200
static uint8_t count_adc;
static uint32_t counter_adc[ROWS][COLUMNS+REF][MEAS];
static uint32_t counter_adcAV[ROWS][COLUMNS+REF];
//static uint32_t last_adc[ROWS][COLUMNS+REF][LAST];
static uint32_t counter_adcMIN[ROWS][COLUMNS+REF];
static uint32_t counter_adcMAX[ROWS][COLUMNS+REF];
uint8_t Matrix_Cap[3] = {5,10,15};


// Function to send a string via USB and check if it was successful
bool printUSB(const char* str) {
	bool success = true;

	while (*str) {
		if (!udi_cdc_putc(*str++)) { // Check if udi_cdc_putc returns success
			success = false;
			break; // Stop if there's a failure
		}
	}

	return success; // Return whether the entire message was successfully sent
}

// Custom printf-like function for USB output with status check
bool usb_printf(const char* format, ...) {
	char buffer[512]; // Buffer to hold the formatted string
	va_list args;     // Variable argument list

	// Initialize the variable argument list
	va_start(args, format);

	// Format the string and store it in the buffer
	vsnprintf(buffer, sizeof(buffer), format, args);

	// Clean up the variable argument list
	va_end(args);

	// Send the formatted string via USB and return success status
	return printUSB(buffer);
}

void send_json_like_message() {
	// Buffer to hold the JSON-like string
    char json_string[10000]; // Adjust the size as needed
    int offset = 0;

    // Construct the JSON-like string without newlines
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "{");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"messageType\":\"DATA\",");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"messageData\":{");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"FWVersion\":\"7.0.0\",");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"matId\":\"MATRIX12345\",");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"numRows\":%d,", ROWS);
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"numCols\":%d,", COLUMNS);
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"activationDate\":\"%s\",", __DATE__);
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"workingHours\":%d,", 1010);
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "\"data\":[");

    // Add the matrix data
    for (int r = 0; r < ROWS; r++) {
        offset += snprintf(json_string + offset, sizeof(json_string) - offset, "[");
        for (int c = 0; c < COLUMNS+REF; c++) { // Use COLUMNS constant here
            offset += snprintf(json_string + offset, sizeof(json_string) - offset, "%d", counter_adcAV[r][c] / LAST);
            if (c < COLUMNS+REF-1) {
                offset += snprintf(json_string + offset, sizeof(json_string) - offset, ",");
            }
        }
        if (r < ROWS - 1) {
            offset += snprintf(json_string + offset, sizeof(json_string) - offset, "],");
        } else {
            offset += snprintf(json_string + offset, sizeof(json_string) - offset, "]");
        }
    }

    // Close the JSON structure
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "]");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "}");
    offset += snprintf(json_string + offset, sizeof(json_string) - offset, "}");
	
	//usb_printf("string length: %d\r\n", strlen(json_string));
    // Send the constructed JSON-like string
    printUSB(json_string);
}





void save_ADC(uint8_t row, uint8_t col, uint8_t sample, uint32_t value)
{
    counter_adc[row][col][sample] = value;
    if (sample >= MEAS-LAST) {
        //last_adc[row][col][sample] = value;
        counter_adcAV[row][col] += value;
        if (value <= counter_adcMIN[row][col]) {
            counter_adcMIN[row][col] = value;
        }
        if (value >= counter_adcMAX[row][col]) {
            counter_adcMAX[row][col] = value;
        }
    }
}

void printADC(uint8_t refNumR, uint8_t refNumC, bool all_matrix, bool just_ref)
{
    if (all_matrix) {
        if (just_ref) {
            for (int c = COLUMNS; c < COLUMNS+REF; c++) {
                usb_printf("Ref%d cap %dpF: average: %d, diff: %d\r\n",
                           c+1-COLUMNS, 
                           Matrix_Cap[c-COLUMNS], 
                           counter_adcAV[0][c]/LAST, 
                           counter_adcMAX[0][c]-counter_adcMIN[0][c]);
            }
            usb_printf("\r\n");
            for (int c = COLUMNS; c < COLUMNS+REF; c++) {
                for (int s = 0; s < MEAS; s++) {
                    counter_adc[0][c][s] = 0;
                }
            }
        } else {
            usb_printf("\r\n");
            for (int r = 0; r < ROWS; r++) {
                usb_printf("r%d,", r+1);
                for (int c = 0; c < COLUMNS+REF; c++) {
                    usb_printf("%d,", counter_adcAV[r][c]/LAST);
                }
                usb_printf("\r\n");
            }
            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLUMNS+REF; c++) {
                    for (int s = MEAS-LAST; s < MEAS; s++) {
                        counter_adc[r][c][s] = 0;
                    }
                }
            }
        }
    } else {
        if (just_ref) {
            usb_printf("R%d C%d average: %d, diff: %d\r\n",
                       refNumR+1, 
                       refNumC+1, 
                       counter_adcAV[refNumR][refNumC]/LAST, 
                       counter_adcMAX[refNumR][refNumC]-counter_adcMIN[refNumR][refNumC]);
        } else {
            usb_printf("ref %d cap %dpF: average: %d, diff: %d\r\n", 
                       refNumC+1, 
                       Matrix_Cap[refNumC], 
                       counter_adcAV[0][COLUMNS + refNumC]/LAST, 
                       counter_adcMAX[0][COLUMNS + refNumC]-counter_adcMIN[0][COLUMNS + refNumC]);
            counter_adc[0][COLUMNS + refNumC][0] = 0;
        }
    }
}

/** AFEC sample data */
struct {
    uint32_t value;
    bool is_done;
} afec_sample_data;

/** The maximal digital value */
static uint32_t g_max_digital = MAX_DIGITAL_12_BIT;

/**
 * \brief AFEC interrupt callback function.
 */
static void afec_data_ready(void)
{
    afec_sample_data.value = afec_get_latest_value(AFEC1);
    afec_sample_data.is_done = true;
}

static void InitADC()
{
    afec_enable(AFEC1);

    struct afec_config afec_cfg;

    afec_get_config_defaults(&afec_cfg);
    afec_init(AFEC1, &afec_cfg);

    struct afec_ch_config afec_ch_cfg;
    afec_ch_get_config_defaults(&afec_ch_cfg);
    afec_ch_set_config(AFEC1, AFEC_CHANNEL_0, &afec_ch_cfg);

    /*
     * Because the internal ADC offset is 0x800, it should cancel it and shift
     * down to 0.
     */
    afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_0, 0x800);
    
    afec_set_trigger(AFEC1, AFEC_TRIG_SW);

    /* Enable channel for potentiometer. */
    afec_channel_enable(AFEC1, AFEC_CHANNEL_0);

    afec_set_callback(AFEC1, AFEC_INTERRUPT_DATA_READY, afec_data_ready, 1);

    afec_start_calibration(AFEC1);
    while ((afec_get_interrupt_status(AFEC1) & AFEC_ISR_EOCAL) != AFEC_ISR_EOCAL);
}

#endif /* CONF_AFEC_H_ */
