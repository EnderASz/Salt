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
 * This is the argument parser for the Salt Virtual Machine.
 *
 * @author bellrise
 */
#ifndef SVM_ARGS_H
#define SVM_ARGS_H

#include "svm.h"

/**
 * Parse the given arguments of the program and set variables depending on
 * the parameters.
 *
 * @param   argc  amount of arguments
 * @param   argv  string array of arguments
 * @return  returns the filename or NULL if filename not found
 */
char *args_parse(SVMRuntime *_rt, i32 argc, char **argv) Nullable;

#endif /* SVM_ARGS_H */

