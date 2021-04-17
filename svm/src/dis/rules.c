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

static String render_value(SVMRuntime *_rt, u8 type, char *payload)
{
    String value = string_new(_rt, 16);

    dprintf("Trying to render value of type %s", get_type_name(type));

    switch (type) {
        case SALT_TYPE_INT:
        {
            snprintf(value.content, 15, "%d", * (u32 *) payload);
            break;
        }
        case SALT_TYPE_FLOAT:
        {
            snprintf(value.content, 15, "%f", * (float *) payload);
            break;
        }
        case SALT_TYPE_NULL:
        {
            snprintf(value.content, 15, "null");
            break;
        }
        case SALT_TYPE_BOOL:
        {
            if (*payload)
                snprintf(value.content, 15, "true");
            else
                snprintf(value.content, 15, "false");
            break;
        }
        case SALT_TYPE_STRING:
        {
            /* Replace unprintable with dots */
            for (u32 i = 0; i < strlen(payload); i++) {
                if (payload[i] < 0x20 || payload[i] > 0x7e)
                    payload[i] = '.';
            }

            snprintf(value.content, 15, "%s", payload);
            break;
        }
        default:
            snprintf(value.content, 15, "?");
    }

    return value;
}

/* Rule functions */

String dis_rule_str(SVMRuntime *_rt, char *payload)
{
    String str = string_new(_rt, 64);
    snprintf(str.content, 63, "'%s'", payload);
    return str;
}

String dis_rule_id(SVMRuntime *_rt, char *payload)
{
    String str = string_new(_rt, 64);
    snprintf(str.content, 63, "%u", * (u32 *) payload);
    return str;
}

String dis_rule_id2(SVMRuntime *_rt, char *payload)
{
    String str = string_new(_rt, 64);
    snprintf(str.content, 63, "%u, %u", * (u32 *) payload,
            * (u32 *) (payload + 4));
    return str;
}

String dis_rule_ivadd(SVMRuntime *_rt, char *payload)
{
    String str = string_new(_rt, 64);
    String value = render_value(_rt, SALT_TYPE_INT, payload + 4);

    snprintf(str.content, 63, "%u, (%s)", * (u32 *) payload, value.content);
    vmfree(value.content, value.size);
    return str;
}

String dis_rule_objmk(SVMRuntime *_rt, char *payload)
{
    u8 type = * (u8 *) (payload + 5);

    String str = string_new(_rt, 64);
    u32 offset = 6;

    /* Omit the 4 length bytes if we're printing a string */
    if (type == SALT_TYPE_STRING)
        offset += 4;

    String value = render_value(_rt, type, payload + offset);

    snprintf(str.content, 63, "%u %s (%s)", * (u32 *) payload,
            get_type_name(type), value.content);

    vmfree(value.content, value.size);

    return str;
}

String dis_rule_rg(SVMRuntime *_rt, char *payload)
{
    u8 reg = * (u8 *) payload;
    u32 id = * (u32 *) (payload + 1);

    String str = string_new(_rt, 16);
    snprintf(str.content, 15, "%u, %u", reg, id);
    return str;
}
