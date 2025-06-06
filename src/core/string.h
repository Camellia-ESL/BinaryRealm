#pragma once

#include <string>

/*
 * String basic type
 */
typedef std::string r_string;

/*
 * WString basic type
 */
typedef std::wstring r_wstring;

/*
 * Get's a string of whatever type, converts it to r_string and then to const
 * char*
 */
#define r_str_to_cstr(str_like) (((r_string)str_like)).c_str()

/*
 * Converts a wstring into an r_string
 */
const r_string wstr_to_rstr(const std::wstring& wstr);

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
