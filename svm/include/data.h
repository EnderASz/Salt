/**
 * This contains global data variables available across the whole Salt Virtual
 * Machine.
 *
 * @author  bellrise
 */
#ifndef SVM_DATA_H
#define SVM_DATA_H

#include "core.h"
#include "module.h"
#include <stddef.h>

/* Name of the compiled salt code (FILE argument) */
extern char *data_filename;

extern unsigned long long data_memory_used;

/* Loaded modules */
extern uint MODULE_AMOUNT;
extern struct SaltModule *MODULES;

#endif // SVM_DATA_H
