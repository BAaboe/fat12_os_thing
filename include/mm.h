#ifndef MM_H
#define MM_H
#include <stdint.h>

#define MM_LOCATION 0x20000

#define MM_TYPE_USABLE 1
#define MM_TYPE_RESERVED 2
#define MM_TYPE_ACPI_RECLAIM_MEM 3
#define MM_TYPE_ACPI_NVS_MEM 4

typedef struct mm_entry {
    uint64_t base_addr;
    uint64_t len;
    uint32_t type;
    uint32_t apci3;
} __attribute__((packed)) mm_entry_t;

typedef struct mm {
    mm_entry_t *entries;
    int len;
} mm_t;

mm_t get_mm();

#endif
