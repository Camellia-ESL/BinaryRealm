/*
 * This file contains all the network imports aligned in order to work properly
 * (especially for win32 that has a strange include pipeline).
 *
 * DO NOT MODIFY THE ORDER, if you don't know what you are doing.
 */

#pragma once
#ifdef _WIN32
  #include <winsock2.h>
  #include <iphlpapi.h>
  #include <Wlanapi.h>
  #include <comdef.h>

  #pragma comment(lib, "Wlanapi.lib")
  #pragma comment(lib, "iphlpapi.lib")
#endif