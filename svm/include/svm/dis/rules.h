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
 * @author bellrise
 */
#ifndef SVM_DIS_RULES_H
#define SVM_DIS_RULES_H

/* These are all the parsing rules for the dissasembler. */

#include <svm/svm.h>


/**
 * All dissasembly rules are presented as a struct, with their instruction
 * and function for parsing.
 */
struct DissasemblyRule
{
    const char instruction[6];
    String (*dis_rule) (SVMRuntime *, char *);
};

/* All these dissasembly rules are responsible for parsing the given payload
 * of the instruction and returning a string that the user can read, providing
 * information about the instruction.
 *
 * @param   payload  instruction payload
 */

String dis_rule_str(SVMRuntime *_rt, char *payload);
String dis_rule_id(SVMRuntime *_rt, char *payload);
String dis_rule_id2(SVMRuntime *_rt, char *payload);
String dis_rule_ivadd(SVMRuntime *_rt, char *payload);
String dis_rule_objmk(SVMRuntime *_rt, char *payload);
String dis_rule_rg(SVMRuntime *_rt, char *payload);


#endif /* SVM_DIS_RULES_H */
