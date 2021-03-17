/**
 * This contains global data variables available across the whole Salt Virtual
 * Machine.
 *
 * @author  bellrise
 */
#ifndef SVM_DATA_H
#define SVM_DATA_H

#include "core.h"
#include <stddef.h>

/* Name of the compiled salt code (FILE argument) */
extern char *data_filename;

extern unsigned long long data_memory_used;

#endif // SVM_DATA_H
