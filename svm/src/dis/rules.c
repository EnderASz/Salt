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
#include <svm/svm.h>
#include <svm/loader.h>
#include <svm/utils.h>

#include <svm/dis/rules.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static const char *type_map[] = {
    "null",
    "int",
    "float",
    "bool",
    "string",
    "model"
};

static const char *get_type_name(u8 id)
{
    if (id >= 6)
        return "unknown";
    return type_map[id];
}

/* Rule functions */

String dis_rule_objmk(SVMRuntime *_rt, char *data)
{
    String str = string_new(_rt, 64);
    String value = string_new(_rt, 16);

    u8 type = * (u8 *) (data + 10);

    switch (type) {
        case SALT_TYPE_INT:
        {
            snprintf(value.content, 15, "%d", * (u32 *) (data + 11));
            break;
        }
        case SALT_TYPE_FLOAT:
        {
            snprintf(value.content, 15, "%f", * (float *) (data + 11));
            break;
        }
        case SALT_TYPE_NULL:
        {
            snprintf(value.content, 15, "null");
            break;
        }
        default:
            snprintf(value.content, 15, "...");
    }

    snprintf(str.content, 63, "%u %s(%s)", * (u32 *) (data + 5),
            get_type_name(type), value.content);

    vmfree(value.content, value.size);

    return str;
}

String dis_rule_print(SVMRuntime *_rt, char *data)
{
    String str = string_new(_rt, 64);
    snprintf(str.content, 63, "%u", * (u32 *) (data + 5));
    return str;
}
