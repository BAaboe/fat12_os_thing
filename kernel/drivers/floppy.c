#include "drivers/floppy.h"
#include "print.h"
#include "time.h"
#include "util.h"
#include <stdint.h>

static volatile uint8_t IRQ6 = 0;
static uint8_t current_drive = 0;
static uint8_t motor_on[2] = {0, 0};

void floppy_read(size_t lba, size_t address, size_t nblocks, uint8_t drive) {

    // Setting up DMA
    outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06); // Mask DRQ
    outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);     // Reseting flip-flop
    outb(DMA_START_ADDRESS_REGISTER_CH_2, (uint8_t)address);
    outb(DMA_START_ADDRESS_REGISTER_CH_2, (uint8_t)(address >> 8));
    outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff); // Reseting flip-flop
    outb(DMA_COUNT_REGISTER_CH_2, (uint8_t)(nblocks * 512 - 1));
    outb(DMA_COUNT_REGISTER_CH_2, (uint8_t)(((nblocks * 512 - 1)) >> 8));
    outb(DMA_PAGE_ADDRESS_REGISTER_CH_2, address >> 16);
    outb(DMA_MODE_REGISTER, 0b01010110);          // DMA mode
    outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02); // Unmask DRQ

    // Sending the read command to FDC
    motor_on[drive] = 1;
    current_drive = drive;
    floppy_drive_select();
    wait(500);

    CHS chs = lba_to_chs(lba, FLOPPY_144_SECTORS_PER_TRACK, FLOPPY_144_HEADS);
    uint16_t result = 0;

    IRQ6 = 0;

    result |= floppy_send_command(FDC_SEEK);
    result |= floppy_send_command_parameter((chs.head << 2) | drive);
    result |= floppy_send_command_parameter(chs.cylinder);
    if (result & 0x100) {
        print_string("Could not seek to cylinder, command error\n", 0x0e);
        return;
    }

    wait_for_irq();

    result = 0;
    result |= floppy_send_command(FDC_SENSE_INTERRUPT);
    result |= floppy_read_command_result();
    if (!(result & (0x20 | drive))) {
        print_string("Could not seek to cylinder\n", 0x0e);
    }
    result &= 0x100;
    result |= floppy_read_command_result();
    if (result & 0x100) {
        print_string("Could not seek to cylinder, command error\n", 0x0e);
        return;
    }

    result |= floppy_send_command(FDC_READ_DATA | 0x80 |
                                  0x40); // read | MT bit | MFM bit
    result |= floppy_send_command_parameter((chs.head << 2) | drive);
    result |= floppy_send_command_parameter(chs.cylinder);
    result |= floppy_send_command_parameter(chs.head);
    result |= floppy_send_command_parameter(chs.sector);
    result |= floppy_send_command_parameter(2);
    result |= floppy_send_command_parameter(18);
    result |= floppy_send_command_parameter(0x1b);
    result |= floppy_send_command_parameter(0xff);
    if (result & 0x100) {
        print_string("Could not read from floppy, command error", 0x0e);
        return;
    }

    // Wait until it is done
    wait_for_irq();

    result = 0;
    // Read result from FDC
    result = 0;
    result |= floppy_read_command_result();
    uint8_t st0 = result & 0xff;
    result &= 0x100;
    result |= floppy_read_command_result();
    uint8_t st1 = result & 0xff;
    result &= 0x100;
    result |= floppy_read_command_result();
    uint8_t st2 = result & 0xff;
    result &= 0x100;
    result |= floppy_read_command_result();
    uint8_t cylinder = result & 0xff;
    result &= 0x100;
    result |= floppy_read_command_result();
    uint8_t ending_head = result & 0xff;
    result &= 0x100;
    result |= floppy_read_command_result();
    uint8_t ending_sector = result & 0xff;
    result &= 0x100;
    result |= floppy_read_command_result();
    uint8_t temp = result & 0xff;
    result &= 0x100;
    outb(0xe9, st1);

    floppy_turn_of_motor(drive);
}

void floppy_init() {
    IRQ6 = 0;
    uint16_t result = 0;
    result |= floppy_send_command(FDC_VERSION);
    result |= floppy_read_command_result();
    if (result & 0x100) {
        print_string("Could not get version of floppy, command error", 0x0e);
        return;
    }
    result &= 0xff;
    if (result != 0x90) {
        print_string("Wrong floppy version", 0x0e);
    }

    floppy_configure();
    result = 0;
    result |= floppy_send_command(FDC_LOCK | 0x80);
    result |= floppy_read_command_result();
    if (result & 0x100) {
        print_string("Could not lock floppy, command error", 0x0e);
        return;
    }

    floppy_reset();

    for (int i = 0; i < 2; i++) {
        motor_on[i] = 1;
        current_drive = i;
        floppy_drive_select();
        wait(500);
        result = 0;
        result |= floppy_send_command(FDC_RECALIBRATE);
        result |= floppy_send_command_parameter((uint8_t)i);
        if (result & 0x100) {
            print_string("Could not calibrate a disk, command error\n", 0x0e);
            return;
        }
        wait_for_irq();

        result = 0;
        result |= floppy_send_command(FDC_SENSE_INTERRUPT);
        result |= floppy_read_command_result();
        if ((result & 0xff) != (0x20 | i)) {
            print_string("Could not calibrate a disk\n", 0x0e);
            return;
        }

        result &= 0x100;
        result |= floppy_read_command_result();
        if (result & 0x100) {
            print_string("Could not calibrate a disk, command error", 0x0e);
        }
        floppy_turn_of_motor(i);
    }
    current_drive = 0;
    floppy_drive_select();
}

void floppy_configure() {
    uint16_t result = 0;
    result |= floppy_send_command(FDC_CONFIGURE);
    result |= floppy_send_command_parameter(0);
    result |= floppy_send_command_parameter(
        (SEEK_ENABLE << 6) | (FIFO_DISABLE << 5) |
        (DRIVE_POLLING_DISABLED << 4) | THRESHOLD - 1);
    result |= floppy_send_command_parameter(0);
    if (result & 0x100) {
        print_string("Could not configure floppy", 0x0e);
        return;
    }
}

void floppy_drive_select() {
    outb(FDC_DATARATE_SELECT_REGISTER, 0x00);
    outb(FDC_DIGITAL_OUTPUT_REGISTER,
         (motor_on[current_drive] << (4 + current_drive)) | 12 | current_drive);
    uint16_t result = 0;
    result |= floppy_send_command(FDC_SPECIFY);
    result |= floppy_send_command_parameter(8 << 4);
    result |= floppy_send_command_parameter(5 << 1);
}

void floppy_turn_of_motor(uint8_t drive) {
    uint8_t tmp = inb(FDC_DIGITAL_OUTPUT_REGISTER);
    outb(FDC_DIGITAL_OUTPUT_REGISTER, tmp & ~(1 << (4 + drive)));
    motor_on[drive] = 0;
}

// Floppy utils
void floppy_reset() {
    uint8_t dor = inb(FDC_DIGITAL_OUTPUT_REGISTER);
    outb(FDC_DIGITAL_OUTPUT_REGISTER, 0x00);
    io_wait();
    outb(FDC_DIGITAL_OUTPUT_REGISTER, dor | 0x4);
    for (int i = 0; i < 2; i++) {
        uint16_t result = 0;
        result |= floppy_send_command(FDC_SENSE_INTERRUPT);
        result |= floppy_read_command_result();
        if (!(result & 0xc0)) {
            print_string("Could not reset floppy", 0x0e);
        }
        result &= 0x100;
        result |= floppy_read_command_result();
        if (result & 0x100) {
            print_string("Could not reset floppy, command error", 0x0e);
        }
    }
}
void wait_for_irq() {
    while (!IRQ6) {
    }
    IRQ6 = 0;
}

uint16_t floppy_send_command(uint8_t command) {
    float start_time = get_milis();
    int error = 1;
    while (get_milis() - start_time < FLOPPY_TIMEOUT_LIMIT) {
        if ((inb(FDC_MAIN_STATUS_REGISTER) & 0xc0) == 0x80) {
            error = 0;
            break;
        }
        floppy_reset();
        floppy_drive_select();
    }

    if (error) {
        print_string("Timeout in command send\n", 0x0e);
        return 0x100;
    }
    outb(FDC_DATA_FIFO, command);
    return 0;
}
uint16_t floppy_send_command_parameter(uint8_t data) {
    float start_time = get_milis();
    int error = 1;
    while (get_milis() - start_time < FLOPPY_TIMEOUT_LIMIT) {
        if (((inb(FDC_MAIN_STATUS_REGISTER) & 0xc0) == 0x80)) {
            error = 0;
            break;
        }
    }
    if (error) {
        print_string("Timeout in command send parameter\n", 0x0e);
        return 0x100;
    }
    outb(FDC_DATA_FIFO, data);
    return 0;
}
uint16_t floppy_read_command_result() {
    float start_time = get_milis();
    int error = 1;
    while (get_milis() - start_time < FLOPPY_TIMEOUT_LIMIT) {
        if (((inb(FDC_MAIN_STATUS_REGISTER) & 0xc0) == 0xc0)) {
            error = 0;
            break;
        }
    }
    if (error) {
        print_string("Timeout in command read\n", 0x0e);
        return 0x100;
    }

    return (uint8_t)inb(FDC_DATA_FIFO);
}

void irq6_handler() { IRQ6 = 1; }
