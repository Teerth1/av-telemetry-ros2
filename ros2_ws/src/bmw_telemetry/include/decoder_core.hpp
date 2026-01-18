#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace telemetry {

    /**
     * @brief Core decoder class for telemetry data.
     */
    class DecoderCore {
    public:
        DecoderCore();
        ~DecoderCore();

        /**
         * @brief Decodes a raw byte buffer.
         * 
         * @param data The raw binary data.
         * @return true if decoding was successful.
         */
        bool decode(const std::vector<uint8_t>& data);
    };

}
