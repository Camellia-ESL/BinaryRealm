#pragma once

#include <string>

/*
 * String basic type
 */
typedef std::string r_string;

/*
 * Get's a string of whatever type, converts it to r_string and then to const
 * char*
 */
#define r_str_to_cstr(str_like) (((r_string)str_like)).c_str()

/*
 * Generate's a uuid string
 */
const r_string r_str_uuid();
