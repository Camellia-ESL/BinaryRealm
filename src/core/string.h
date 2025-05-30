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

/*
 * Get's the local date already formated in string
 */
const r_string r_date_and_time_as_str();

/*
 * Get's the local date (without time) already formated in string
 */
const r_string r_date_as_str();

/*
 * Get's the current time already formated in string
 */
const r_string r_time_as_str();
