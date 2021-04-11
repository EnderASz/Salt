/**
 * Salt Virtual Machine
 * 
 * Copyright (C) 2021  The Salt Programming Language Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * END OF COPYRIGHT NOTICE
 *
 * The svm loader is responsible for loading a file and adding it to
 * the global module list.
 *
 * @author  bellrise
 */
#ifndef SVM_LOADER_H
#define SVM_LOADER_H

#include "svm.h"
#include "module.h"

/**
 * Load the given compiled salt code into the global module list. Here is
 * a shortened version of the steps load() does:
 *  - read the module
 *  - validate the header
 *  - allocate space for a new salt module
 *  - load symbols from bytecode
 *
 * @param   name    name of the module
 * @returns pointer to acquired SaltModule
 */
struct SaltModule *load(SVMRuntime *_rt, char *name);

/**
 * Load an external module from the salt home /ext directory OR the local
 * /etx directory. The local ext directory has priority over the latter.
 * If the module with the given name (without the .scc extension) cannot 
 * be found, it throws an runtime exception.
 *
 * This function automatically handles lookups in the runtime module array
 * and memory allocation for it. If it finds an already imported module with
 * the same name, it returns the pointer to that module, and does not load 
 * a new one.
 * 
 * @param   name    name of the module to load
 * @returns pointer to acquired SaltModule
 */
struct SaltModule *ext_load(SVMRuntime *_rt, char *name);

#endif // SVM_LOADER_H
