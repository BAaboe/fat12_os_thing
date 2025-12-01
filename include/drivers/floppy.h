#ifndef FLOPPY_H
#define FLOPPY_H
#include "util.h"
#include <stddef.h>
#include <stdint.h>

#define FLOPPY_144_SECTORS_PER_TRACK 18
#define FLOPPY_144_HEADS 2
#define FLOPPY_144_TRACKS_PER_HEAD 80

#define FLOPPY_TIMEOUT_LIMIT 4000

// Copied from https://wiki.osdev.org/Floppy_Disk_Controller
#define FDC_READ_TRACK 2 // generates IRQ6
#define FDC_SPECIFY 3    // * set drive parameters
#define FDC_SENSE_DRIVE_STATUS 4
#define FDC_WRITE_DATA 5      // * write to the disk
#define FDC_READ_DATA 6       // * read from the disk
#define FDC_RECALIBRATE 7     // * seek to cylinder 0
#define FDC_SENSE_INTERRUPT 8 // * ack IRQ6, get status of last command
#define FDC_WRITE_DELETED_DATA 9
#define FDC_READ_ID 10 // generates IRQ6
#define FDC_READ_DELETED_DATA 12
#define FDC_FORMAT_TRACK 13 // *
#define FDC_DUMPREG 14
#define FDC_SEEK 15    // * seek both heads to cylinder X
#define FDC_VERSION 16 // * used during initialization, once
#define FDC_SCAN_EQUAL 17
#define FDC_PERPENDICULAR_MODE 18 // * used during initialization, once, maybe
#define FDC_CONFIGURE 19          // * set controller parameters
#define FDC_LOCK 20               // * protect controller params from a reset
#define FDC_VERIFY 22
#define FDC_SCAN_LOW_OR_EQUAL 25
#define FDC_SCAN_HIGH_OR_EQUAL 29

#define FDC_STATUS_REGISTER_A 0x3F0 // read-only
#define FDC_STATUS_REGISTER_B 0x3F1 // read-only
#define FDC_DIGITAL_OUTPUT_REGISTER 0x3F2
#define FDC_TAPE_DRIVE_REGISTER 0x3F3
#define FDC_MAIN_STATUS_REGISTER 0x3F4     // read-only
#define FDC_DATARATE_SELECT_REGISTER 0x3F4 // write-only
#define FDC_DATA_FIFO 0x3F5
#define FDC_DIGITAL_INPUT_REGISTER 0x3F7         // read-only
#define FDC_CONFIGURATION_CONTROL_REGISTER 0x3F7 // write-only

#define DMA_START_ADDRESS_REGISTER_CH_1 0x02  // write-only
#define DMA_COUNT_REGISTER_CH_1 0x03          // write-only
#define DMA_START_ADDRESS_REGISTER_CH_2 0x04  // write-only
#define DMA_COUNT_REGISTER_CH_2 0x05          // write-only
#define DMA_START_ADDRESS_REGISTER_CH_3 0x06  // write-only
#define DMA_COUNT_REGISTER_CH_3 0x07          // write-only
#define DMA_STATUS_REGISTER 0x08              // read-only
#define DMA_COMMAND_REGISTER 0x08             // write-only
#define DMA_REQUEST_REGISTER 0x09             // write-only
#define DMA_SINGLE_CHANNEL_MASK_REGISTER 0x0a // write-only
#define DMA_MODE_REGISTER 0x0b                // write-only
#define DMA_FLIP_FLOP_RESET_REGISTER 0x0c     // write-only
#define DMA_INTERMEDIAT_REGISTER 0x0d         // read-only
#define DMA_MASTER_RESET_REGISTER 0x0d        // write-only
#define DMA_MASK_RESET_REGISTER 0x0e          // write-only
#define DMA_MULTICHANNEL_MASK_REGISTER 0x0f

#define DMA_PAGE_ADDRESS_REGISTER_CH_1 0x83
#define DMA_PAGE_ADDRESS_REGISTER_CH_2 0x81
#define DMA_PAGE_ADDRESS_REGISTER_CH_3 0x82

// Config
#define SEEK_ENABLE 0
#define FIFO_DISABLE 0
#define DRIVE_POLLING_DISABLED 1
#define THRESHOLD 8

// IRQ6
void irq6_handler();

void floppy_init();
void floppy_drive_select();
void floppy_turn_of_motor(uint8_t drive);
void floppy_read(size_t lba, size_t address, size_t size, uint8_t drive);

// Floppy util functions
void wait_for_irq();
void floppy_reset();

uint16_t floppy_send_command(uint8_t command);
uint16_t floppy_send_command_parameter(uint8_t data);
uint16_t floppy_read_command_result();

void floppy_configure();
#endif
