#include "stdinclude.hpp"
#include "sdvx_hardware.hpp"

namespace component
{
    namespace sdvx
    {

        void start()
        {
            startup();
        }

        void update()
        {
            buttons();
            encFuncLeft();
            encFuncRight();
            moveLed();
        }
    }
}