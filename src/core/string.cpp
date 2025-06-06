#include "string.h"

#include <codecvt>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <random>
#include <sstream>

const r_string wstr_to_rstr(const std::wstring& wstr) {
  static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

const r_string r_str_uuid() {
  // Random Generator Setup for uuid string
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(0, 15);
  static std::uniform_int_distribution<> dis2(8, 11);
  std::stringstream ss;
  int i;
  ss << std::hex;
  for (i = 0; i < 8; i++) {
    ss << dis(gen);
  }
  ss << "-";
  for (i = 0; i < 4; i++) {
    ss << dis(gen);
  }
  ss << "-4";
  for (i = 0; i < 3; i++) {
    ss << dis(gen);
  }
  ss << "-";
  ss << dis2(gen);
  for (i = 0; i < 3; i++) {
    ss << dis(gen);
  }
  ss << "-";
  for (i = 0; i < 12; i++) {
    ss << dis(gen);
  };
  return ss.str();
}

const r_string r_date_and_time_as_str() {
  std::time_t now = std::time(nullptr);
  std::tm* localTime = std::localtime(&now);

  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << localTime->tm_mday << "/"
      << std::setw(2) << (localTime->tm_mon + 1) << "/"
      << (1900 + localTime->tm_year) << " - " << std::setw(2)
      << localTime->tm_hour << ":" << std::setw(2) << localTime->tm_min;

  return oss.str();
}

const r_string r_date_as_str() {
  std::time_t now = std::time(nullptr);
  std::tm* localTime = std::localtime(&now);

  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << localTime->tm_mday << "/"
      << std::setw(2) << (localTime->tm_mon + 1) << "/"
      << (1900 + localTime->tm_year);

  return oss.str();
}

const r_string r_time_as_str() {
  std::time_t now = std::time(nullptr);
  std::tm* localTime = std::localtime(&now);

  int hour = localTime->tm_hour;
  std::string period = (hour >= 12) ? "PM" : "AM";
  hour = hour % 12;
  if (hour == 0) hour = 12;  // Handle midnight and noon

  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2)
      << localTime->tm_min << " " << period;

  return oss.str();
}
