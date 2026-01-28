#ifndef UTILS_H
#define UTILS_H

#include "types.h"

extern uint32 strlen(const char*);
extern uint32 digit_count(int);
extern void itoa(int, char *);

// Disk detection functions
extern uint32 detect_disks(uint32* master_size, uint32* slave_size);

#endif

