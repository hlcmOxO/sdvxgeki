#include "stdinclude.hpp"

namespace component {
    namespace raw_hid {
        uint8_t rawBuffer[255];
        uint8_t outBuffer[64];
        uint8_t inBuffer[64];

        output_data_t *pOutputData = reinterpret_cast<output_data_t *>(outBuffer);
        input_data_t *pInputData = reinterpret_cast<input_data_t *>(inBuffer);

        void start() {
            RawHID.begin(rawBuffer, sizeof(rawBuffer));
        }

        void update() {
            ongeki_hardware::read_io(pOutputData);
            RawHID.write(outBuffer, 64);

            if (RawHID.available()) {
                RawHID.readBytes(inBuffer, 64);

                switch (pInputData->type) {
                    case 0: {
                        ongeki_hardware::set_led(pInputData->led);
                        break;
                    }
                    case 1: {
                        break;
                    }
                }
            }
        }
    }
}
