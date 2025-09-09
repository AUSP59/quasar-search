// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace quasar::log {
inline void info(const std::string& m){ std::cerr << "[INFO] " << m << "\n"; }
inline void warn(const std::string& m){ std::cerr << "[WARN] " << m << "\n"; }
inline void error(const std::string& m){ std::cerr << "[ERROR] " << m << "\n"; }
} // namespace quasar::log