# Autonomous Telemetry Core

## Overview
This project contains the core decoding logic for the autonomous telemetry system.

## Project Structure
```
autonomous-telemetry-core/
├── src/
│   └── decoder_core.cpp      # Implementation of the decoder
├── include/
│   └── decoder_core.hpp      # Header definitions
├── CMakeLists.txt            # Build configuration
└── README.md                 # Project documentation
```

## Building
To build the project using CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
Include `decoder_core.hpp` in your application to use the `telemetry::DecoderCore` class.
