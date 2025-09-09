// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

namespace quasar::metrics_export {

inline void write_prom(const std::string& path,
                       size_t files, uint64_t bytes,
                       double elapsed_ms, double p95_ms,
                       double tps, double mbps) {
  std::ofstream out(path, std::ios::binary);
  if (!out) return;
  out << "# HELP quasar_files Number of files scanned\n";
  out << "# TYPE quasar_files counter\n";
  out << "quasar_files " << files << "\n";
  out << "# HELP quasar_bytes Total bytes scanned\n";
  out << "# TYPE quasar_bytes counter\n";
  out << "quasar_bytes " << bytes << "\n";
  out << "# HELP quasar_elapsed_ms Total elapsed milliseconds\n";
  out << "# TYPE quasar_elapsed_ms gauge\n";
  out << "quasar_elapsed_ms " << elapsed_ms << "\n";
  out << "# HELP quasar_p95_ms 95th percentile per-file latency\n";
  out << "# TYPE quasar_p95_ms gauge\n";
  out << "quasar_p95_ms " << p95_ms << "\n";
  out << "# HELP quasar_tps Files per second\n";
  out << "# TYPE quasar_tps gauge\n";
  out << "quasar_tps " << tps << "\n";
  out << "# HELP quasar_mbps MiB per second\n";
  out << "# TYPE quasar_mbps gauge\n";
  out << "quasar_mbps " << mbps << "\n";
}

} // namespace quasar::metrics_export