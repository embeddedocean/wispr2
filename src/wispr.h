/*
 * wispr.h
 *
 * Created: 12/24/2019 4:59:31 AM
 *  Author: Chris
 */ 

#ifndef WISPR_H_
#define WISPR_H_

#include "epoch.h"
//#include "sd_card.h"

#define WISPR_VERSION 2
#define WISPR_SUBVERSION 0

#define WISPR_SD_CARD_HEADER_BLOCK (30)
#define WISPR_SD_CARD_CONFIG_BLOCK (31)
#define WISPR_SD_CARD_START_BLOCK (32)

// sd cards only read in blocks, not bytes, 
// so reads and writes must be done in blocks of this size
// fixed at 512 bytes for standard SD cards
#define WISPR_SD_CARD_BLOCK_SIZE (512)

#define WISPR_FILE_SIZE (512)

// The header size should be a multiple of a word size (4 bytes)
// to avoid issues with word alignment when casting
#define WISPR_DATA_HEADER_SIZE (32)

// Fixed values used to define static buffers
#define ADC_SAMPLE_SIZE (3)
#define ADC_BLOCKS_PER_BUFFER (36)
#define ADC_BUFFER_SIZE (ADC_BLOCKS_PER_BUFFER * WISPR_SD_CARD_BLOCK_SIZE)

// if the header is NOT included in adc buffer use this
#define ADC_MAX_SAMPLES_PER_BUFFER (ADC_BUFFER_SIZE / ADC_SAMPLE_SIZE)

// if the header is included in adc buffer use this
//#define ADC_SAMPLES_PER_BUFFER ((ADC_BUFFER_SIZE - WISPR_DATA_HEADER_SIZE) / ADC_SAMPLE_SIZE)

// adc default values
#define ADC_DEFAULT_SAMPLING_RATE (50000)
#define ADC_DEFAULT_SAMPLE_SIZE (3)
#define ADC_DEFAULT_AWAKE 10
#define ADC_DEFAULT_SLEEP 10
#define ADC_DEFAULT_GAIN 0
#define ADC_DEFAULT_DECIMATION 4

// dma status flags
#define ADC_DMA_OVERFLOW 0x01
#define ADC_DMA_BUFFER_OVERRUN 0x02
#define ADC_DMA_MISSED_BUFFER 0x04

// adc reference voltage
#define ADC_VREF 5.0
#define ADC_SCALING 5.0

// spectrum max values
#define PSD_FFT_SIZE (1024)
#define PSD_BLOCKS_PER_BUFFER (4) 
#define PSD_BUFFER_SIZE (PSD_BLOCKS_PER_BUFFER * WISPR_SD_CARD_BLOCK_SIZE) 

#define PSD_MAX_FFT_SIZE (1024)

// if the header is NOT included in psd buffer use this
#define PSD_MAX_BINS_PER_BUFFER (PSD_BUFFER_SIZE / 4)

// if the header is included in psd buffer use this
//#define PSD_MAX_BINS_PER_BUFFER ((PSD_BUFFER_SIZE - WISPR_DATA_HEADER_SIZE) / 4)

// States
#define WISPR_READING_ADC 0x01
#define WISPR_WRITING_SD1 0x02
#define WISPR_WRITING_SD2 0x04
#define WISPR_POWER_OFF 0x10
#define WISPR_READY 0x20
#define WISPR_BOOTING 0x40
#define WISPR_SHUTTING_DOWN 0x80

// Modes
#define WISPR_WAVEFORM 0x01
#define WISPR_SPECTRUM 0x02

// Data types
#define WISPR_INT16 0x01
#define WISPR_INT24 0x02
#define WISPR_INT32 0x04
#define WISPR_FLOAT16 0x10
#define WISPR_FLOAT32 0x20

// Max file size is in units of 512 blocks
#define WISPR_MAX_FILE_SIZE (ADC_BLOCKS_PER_BUFFER * 3052)

// settings array index for different data header types
#define ADC_SETTINGS_INDEX_GAIN 0
#define ADC_SETTINGS_INDEX_DF 1
#define ADC_SETTINGS_INDEX_2 2
#define ADC_SETTINGS_INDEX_3 3

#define PSD_SETTINGS_INDEX_SAMPLE_SIZE 0
#define PSD_SETTINGS_INDEX_FFT_WINTYPE 1
#define PSD_SETTINGS_INDEX_FFT_SIZE 2
#define PSD_SETTINGS_INDEX_FFT_OVERLAP 3

// Bit shifting macro with casting to load an int32 from a little-endian buffer containing an int24
#define LOAD_INT24(u8,n) ((int32_t)(((uint32_t)u8[3*n+0] << 8) | ((uint32_t)u8[3*n+1] << 16) | ((uint32_t)u8[3*n+2] << 24)) >> 8)

// Bit shifting macro with casting to load an int32 from a little-endian buffer containing an int16
#define LOAD_INT16(u8,n) ((int16_t)(((uint16_t)u8[2*n+0] << 0) | ((uint16_t)u8[2*n+1] << 8)))

//
// Data header object written to the front of each data buffer
//
typedef struct {
	char      name[6];
	uint8_t   version[2];
	uint8_t   type;
	uint32_t  second; // epoch time stamp
	uint32_t  usec;
	uint8_t   settings[4];
	uint8_t   sample_size; // number of bytes per sample
	uint16_t  buffer_size; // number of bytes in an adc data buffer
	uint16_t  samples_per_buffer;  // number of samples in a buffer
	uint32_t  sampling_rate; // samples per second
	uint8_t   channels;  // number of channels
	uint8_t   header_chksum;
	uint8_t   data_chksum;
} wispr_data_header_t;

//
// Configuration data object written to the configuration block of the sd card 
//
typedef struct {
	char     name[6];
	uint8_t  version[2];
	uint8_t  state;
	uint8_t  mode;
	uint32_t epoch; // linux time in seconds
	uint8_t  sample_size; // number of bytes per sample
	uint16_t buffer_size;  // number of bytes in a buffer, this can be different than samples_per_buffer*sample_size
	uint16_t samples_per_buffer; // number of samples in a buffer
	uint32_t sampling_rate; // samples per second
	//uint8_t  settings[8]; // various system and adc settings
	uint8_t  channels;  // number of channels
	uint8_t  gain;  // preamp gain
	uint8_t  adc_decimation;  // 4, 8, 13, or 32
	uint16_t buffers_per_window; // number of adc record buffers in a sampling window
	uint16_t acquisition_time; // time in seconds of the adc sampling window
	uint16_t sleep_time; // time in seconds between adc records (must be >= window)
	uint16_t fft_size; // fft size used for spectrum
	uint16_t fft_overlap; // fft overlap used for spectrum
	uint16_t fft_window_type;
	uint32_t file_size; // number of block (512 byte) per file
	uint8_t active_sd_card; // last card written to
} wispr_config_t;


extern int wispr_parse_data_header(uint8_t *buf, wispr_data_header_t *hdr);
extern int wispr_serialize_data_header(wispr_data_header_t *hdr, uint8_t *buf);
extern void wispr_print_data_header(wispr_data_header_t *header);
extern void wispr_print_config(wispr_config_t *hdr);
extern int wispr_parse_config(uint8_t *buf, wispr_config_t *hdr);
extern int wispr_serialize_config(wispr_config_t *hdr, uint8_t *buf);
extern void wispr_update_data_header(wispr_config_t *wispr, wispr_data_header_t *hdr);

#ifdef WINDOWS
typedef struct {
	char     label[16];     // user set identifier
	uint8_t  number;        // card number
	uint8_t  version[2];    // wispr version
	uint8_t  hw_ver;        // card hardware version
	uint8_t  state;         // current state (OPEN, ENABLED, ..)
	uint8_t  type;          // hardware type
	uint32_t capacity;      // total card capacity in KBytes
	uint32_t start;         // addr of start block
	uint32_t end;           // addr of end block
	uint32_t write_addr;    // addr of current write block
	uint32_t read_addr;     // addr of current read block
	uint32_t total;         // total number of sectors KBytes
	uint32_t free;          // available sectors in KBytes
	uint32_t epoch;         // mod time
} wispr_sd_card_t;

extern void wispr_print_sd_card_header(wispr_sd_card_t *hdr);
extern int wispr_sd_card_parse_header(uint8_t *buf, wispr_sd_card_t *hdr);
extern int wispr_sd_card_serialize_header(sd_card_t *hdr, uint8_t *buf);
#endif

//extern int wispr_gpbr_write_config(wispr_config_t *hdr);
//extern int wispr_gpbr_read_config(wispr_config_t *hdr);

#endif /* WISPR_H_ */
