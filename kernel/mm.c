#include "mm.h"
#include <stdint.h>

mm_t get_mm() {
    uint16_t *len = (uint16_t *)MM_LOCATION;
    mm_t mm;
    mm.len = *len;
    mm.entries = (mm_entry_t *)(MM_LOCATION + 24);
    return mm;
}
