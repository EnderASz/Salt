/**
 * The svm loader is responsible for loading a file and adding it to
 * the global module list.
 *
 * @author  bellrise
 */
#ifndef SVM_LOADER_H
#define SVM_LOADER_H

#include "core.h"

/**
 * Load the given compiled salt code into the global module list. Here is
 * a shortened version of the steps load() does:
 *  - read the module
 *  - validate the header
 *  - allocate space for a new salt module
 *  - load symbols from bytecode
 *
 * @param   name name of the module
 * @param   alternative alternative name
 * @returns load status, 0 for failed 1 for success
 */
int load(char *name, const char *alternative);

#endif // SVM_LOADER_H