#include "util.h"

void debug_print(char *string) {
    while (*string != 0) {
        outb(0xe9, *string++);
    }
}

CHS lba_to_chs(int lba, int spt, int hpc) {
    CHS chs = {.cylinder = lba / (spt * hpc),
               .head = (lba % (spt * hpc)) / (spt),
               .sector = (lba % spt) + 1};

    return chs;
}
