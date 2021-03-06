/*
 * This file is derived from the MicroPython project, http://micropython.org/
 *
 * Copyright (c) 2016, Pycom Limited and its licensors.
 *
 * This software is licensed under the GNU GPL version 3 or any later version,
 * with permitted additional terms. For more information see the Pycom Licence
 * v1.0 document supplied with this file, or available at:
 * https://www.pycom.io/opensource/licensing
 */

#include <stdio.h>

#include "py/mpconfig.h"
#include "py/obj.h"

STATIC const char help_text[] = "Welcome to MicroPython!\n"
                                "For online help please visit http://micropython.org/help/.\n"
                                "For further help on a specific object, type help(obj)\n";

STATIC void pyb_help_print_info_about_object(mp_obj_t name_o, mp_obj_t value) {
    mp_printf(&mp_plat_print, "  ");
    mp_obj_print(name_o, PRINT_STR);
    mp_printf(&mp_plat_print, " -- ");
    mp_obj_print(value, PRINT_STR);
    mp_printf(&mp_plat_print, "\n");
}

STATIC mp_obj_t pyb_help(uint n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        // print a general help message
        printf("%s", help_text);
    }
    else {
        // try to print something sensible about the given object
        printf("object ");
        mp_obj_print(args[0], PRINT_STR);
        printf(" is of type %s\n", mp_obj_get_type_str(args[0]));

        mp_map_t *map = NULL;
        if (MP_OBJ_IS_TYPE(args[0], &mp_type_module)) {
            map = mp_obj_dict_get_map(mp_obj_module_get_globals(args[0]));
        } else {
            mp_obj_type_t *type;
            if (MP_OBJ_IS_TYPE(args[0], &mp_type_type)) {
                type = args[0];
            } else {
                type = mp_obj_get_type(args[0]);
            }
            if (type->locals_dict != MP_OBJ_NULL && MP_OBJ_IS_TYPE(type->locals_dict, &mp_type_dict)) {
                map = mp_obj_dict_get_map(type->locals_dict);
            }
        }
        if (map != NULL) {
            for (uint i = 0; i < map->alloc; i++) {
                if (map->table[i].key != MP_OBJ_NULL) {
                    pyb_help_print_info_about_object(map->table[i].key, map->table[i].value);
                }
            }
        }
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_builtin_help_obj, 0, 1, pyb_help);
