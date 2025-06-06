#include "network.h"

#include <memory>

RNetworkAdaptersManager::RNetworkAdaptersManager() {
  refresh_adapters();
  last_updated_ = std::chrono::steady_clock::now();
}

void RNetworkAdaptersManager::refresh_adapters() {
  adapters_.clear();
  last_snapshots_.clear();

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

    MIB_IFROW row = {};
    row.dwIndex = addr->IfIndex;

    if (GetIfEntry(&row) != NO_ERROR) continue;

    adapters_.emplace_back(
        RNetAdapter{wstr_to_rstr(addr->FriendlyName), addr->IfIndex, 0.0, 0.0});

    last_snapshots_.emplace_back(
        NetSnapshot{addr->IfIndex, row.dwInOctets, row.dwOutOctets});
  }
}

void RNetworkAdaptersManager::update() {
  const auto now = std::chrono::steady_clock::now();
  const double seconds =
      std::chrono::duration<double>(now - last_updated_).count();
  if (seconds <= 0.0) return;

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
