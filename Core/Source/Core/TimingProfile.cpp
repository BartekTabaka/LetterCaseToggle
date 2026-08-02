#include "TimingProfile.h"

namespace Core
{
    namespace
    {
        constexpr TimingProfile cNormal{ /*settleWait*/ 30, /*copyTimeout*/ 80, /*ctrlCTimeout*/ 150, /*pasteWait*/ 120 };
        constexpr TimingProfile cFast{ /*settleWait*/ 15, /*copyTimeout*/ 50, /*ctrlCTimeout*/ 90, /*pasteWait*/ 60 };
    }

    const TimingProfile& Core::GetTimingProfile(Speed speed)
    {
        switch (speed) {
            case Core::Speed::Normal:
                return cNormal;
            case Core::Speed::Fast:
                return cFast;
            default:
                return cNormal;
        }
    }
}
