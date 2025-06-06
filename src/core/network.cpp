#include "network.h"

#include <memory>
#include <unordered_map>

RNetworkAdaptersManager::RNetworkAdaptersManager() {
  refresh_adapters();
  last_updated_ = std::chrono::steady_clock::now();
}

void RNetworkAdaptersManager::refresh_adapters() {
  adapters_.clear();
  last_snapshots_.clear();

  // Get all connected Wi-Fi SSIDs via WLAN API
  std::unordered_map<r_string, r_string> wifi_ssids;
  HANDLE hClient = nullptr;
  DWORD negotiated_version = 0;

  if (WlanOpenHandle(2, nullptr, &negotiated_version, &hClient) ==
      ERROR_SUCCESS) {
    PWLAN_INTERFACE_INFO_LIST pIfList = nullptr;
    if (WlanEnumInterfaces(hClient, nullptr, &pIfList) == ERROR_SUCCESS) {
      for (int i = 0; i < (int)pIfList->dwNumberOfItems; ++i) {
        const WLAN_INTERFACE_INFO& iface = pIfList->InterfaceInfo[i];
        WLAN_CONNECTION_ATTRIBUTES* pConnectInfo = nullptr;
        DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
        WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

        if (WlanQueryInterface(hClient, &iface.InterfaceGuid,
                               wlan_intf_opcode_current_connection, nullptr,
                               &connectInfoSize, (PVOID*)&pConnectInfo,
                               &opCode) == ERROR_SUCCESS) {
          if (pConnectInfo->isState == wlan_interface_state_connected) {
            const DOT11_SSID& ssid =
                pConnectInfo->wlanAssociationAttributes.dot11Ssid;
            r_string ssid_str(reinterpret_cast<const char*>(ssid.ucSSID),
                              ssid.uSSIDLength);
            r_string interface_desc =
                wstr_to_rstr(std::wstring(iface.strInterfaceDescription));
            wifi_ssids[interface_desc] = ssid_str;
          }
          WlanFreeMemory(pConnectInfo);
        }
      }
      WlanFreeMemory(pIfList);
    }
    WlanCloseHandle(hClient, nullptr);
  }

  // Now fetch standard network info
  ULONG buffer_size = 0;
  GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, nullptr, &buffer_size);
  auto buffer = std::make_unique<BYTE[]>(buffer_size);
  IP_ADAPTER_ADDRESSES* p_addrs =
      reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.get());

  if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, p_addrs, &buffer_size) !=
      NO_ERROR)
    return;

  for (auto* addr = p_addrs; addr != nullptr; addr = addr->Next) {
    if (addr->OperStatus != IfOperStatusUp) continue;
    if (addr->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;

    MIB_IFROW row = {};
    row.dwIndex = addr->IfIndex;
    if (GetIfEntry(&row) != NO_ERROR) continue;

    r_string net_name;
    if (addr->Description != nullptr) {
      r_string description_str = wstr_to_rstr(std::wstring(addr->Description));
      auto it = wifi_ssids.find(description_str);
      if (it != wifi_ssids.end()) {
        net_name = it->second;
      }
    }

    adapters_.emplace_back(
        RNetAdapter{wstr_to_rstr(std::wstring(addr->FriendlyName)), net_name,
                    addr->IfIndex, 0.0, 0.0});

    last_snapshots_.emplace_back(
        NetSnapshot{addr->IfIndex, row.dwInOctets, row.dwOutOctets});
  }
}

void RNetworkAdaptersManager::update() {
  const auto now = std::chrono::steady_clock::now();
  const double seconds =
      std::chrono::duration<double>(now - last_updated_).count();
  const double refresh_seconds =
      std::chrono::duration<double>(now - last_refresh_).count();
  if (seconds <= 1.0) return;
  if (refresh_seconds >= 30.0) {
    refresh_adapters();
    last_refresh_ = now;
  }

  for (size_t i = 0; i < adapters_.size(); ++i) {
    MIB_IFROW row = {};
    row.dwIndex = adapters_[i].index;

    if (GetIfEntry(&row) != NO_ERROR) continue;

    const auto& last = last_snapshots_[i];
    const ULONG64 curr_in = row.dwInOctets;
    const ULONG64 curr_out = row.dwOutOctets;

    adapters_[i].download_kbps =
        ((curr_in - last.in_octets) * 8.0) / (seconds * 1'000.0);
    adapters_[i].upload_kpbs =
        ((curr_out - last.out_octets) * 8.0) / (seconds * 1'000.0);

    last_snapshots_[i].in_octets = curr_in;
    last_snapshots_[i].out_octets = curr_out;
  }

  last_updated_ = now;
}

const std::vector<RNetAdapter>& RNetworkAdaptersManager::get_adapters() const {
  return adapters_;
}
