<p align="center">
  <h1 align="center">🚗 Autonomous Telemetry Core</h1>
  <p align="center">
    <strong>High-performance C++ library for decoding autonomous vehicle telemetry data</strong>
  </p>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=cplusplus" alt="C++17"/>
  <img src="https://img.shields.io/badge/Build-CMake-064F8C.svg?style=for-the-badge&logo=cmake" alt="CMake"/>
  <img src="https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge" alt="MIT License"/>
  <img src="https://img.shields.io/badge/Status-Active-success.svg?style=for-the-badge" alt="Active"/>
</p>

---

## 📋 Overview

**Autonomous Telemetry Core** is the foundational decoding engine designed for processing raw binary telemetry streams from autonomous vehicle systems. Built with modern C++17, it provides a robust, modular, and extensible architecture for integrating into larger systems like ROS2 nodes.

This library handles the critical task of parsing incoming sensor data packets, enabling real-time analytics and decision-making pipelines.

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🔧 **Modern C++17** | Leverages modern language features for performance and safety |
| 📦 **CMake Integration** | Cross-platform build system ready to go |
| 🧱 **Modular Design** | Clean separation of headers and implementation |
| 🔌 **Extensible** | Easy to integrate with ROS2 or other middleware |
| ⚡ **High Performance** | Optimized for real-time data processing |

---

## 🛠️ Getting Started

### Prerequisites

- **C++ Compiler** supporting C++17 (GCC 7+, Clang 5+, or MSVC 2017+)
- **CMake** 3.10 or higher

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/Teerth1/autonomous-telemetry-core.git
cd autonomous-telemetry-core

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .
```

### Running the Demo

After building, run the telemetry decoder demo:

```bash
# Windows
.\Debug\telemetry_decoder.exe

# Linux/macOS
./telemetry_decoder
```

**Expected Output:**
```
Starting Telemetry Runner...
[DecoderCore] Processing 4 bytes.
Decoding successful.
```

---

## 📁 Project Structure

```
autonomous-telemetry-core/
├── 📂 src/
│   ├── main.cpp              # Demo application entry point
│   └── decoder_core.cpp      # Core decoder implementation
├── 📂 include/
│   └── decoder_core.hpp      # Public header definitions
├── 📂 build/                  # Build output (generated)
├── 📄 CMakeLists.txt          # Build configuration
├── 📄 LICENSE                 # MIT License
└── 📄 README.md               # This file
```

---

## 💻 Usage Example

```cpp
#include <vector>
#include "decoder_core.hpp"

int main() {
    telemetry::DecoderCore decoder;
    
    // Your raw telemetry data
    std::vector<uint8_t> raw_data = {0x01, 0x02, 0x03, 0x04};
    
    if (decoder.decode(raw_data)) {
        // Process decoded telemetry
    }
    
    return 0;
}
```

---

## 🗺️ Roadmap

- [ ] Implement full telemetry packet parsing
- [ ] Add support for multiple sensor protocols
- [ ] ROS2 node integration
- [ ] Unit test coverage
- [ ] Performance benchmarking

---

## 🤝 Contributing

Contributions are welcome! Feel free to:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

<p align="center">
  Made with ❤️ by <strong>Teerth Patel</strong>
</p>
