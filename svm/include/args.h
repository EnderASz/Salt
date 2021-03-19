/**
 * This is the argument parser for the Salt Virtual Machine.
 *
 * @author bellrise
 */
#ifndef SVM_ARGS_H
#define SVM_ARGS_H

#include "core.h"

/**
 * Parse the given arguments of the program and set variables depending on
 * the parameters.
 *
 * @param argc amount of arguments
 * @param argv string array of arguments
 */
char __NULLABLE *args_parse(int argc, char **argv);

#endif // SVM_ARGS_H
