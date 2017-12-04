#include "reset_reason_api.h"

#include "fsl_rcm.h"

reset_reason_t hal_reset_reason_get(void)
{
    const uint32_t reset_sources =
        RCM_GetPreviousResetSources(RCM) & kRCM_SourceAll;

    // Low power mode is exited via the RESET pin. Therefore, when this reset is
    // triggered both the PIN and WAKEUP will have bits set, so check this flag
    // first.
#if (defined(FSL_FEATURE_RCM_HAS_WAKEUP) && FSL_FEATURE_RCM_HAS_WAKEUP)
    if ((reset_sources & kRCM_SourceWakeup) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

    // Check POR flag first. During a POR reset there will be two reset sources
    // set: POR and LVD. As during the power on phase the low voltage detector
    // circuit will detect a low voltage while the voltage is initially ramping
    // up and set the BROWN_OUT flag. Therefore, if LVD is set we must check the
    // POR to determine what the actual cause was.
    if ((reset_sources & kRCM_SourcePor) != 0) {
        return RESET_REASON_POWER_ON;
    }

    if ((reset_sources & kRCM_SourceLvd) != 0) {
        return RESET_REASON_BROWN_OUT;
    }

    if ((reset_sources & kRCM_SourceWdog) != 0) {
        return RESET_REASON_WATCHDOG;
    }

    if ((reset_sources & kRCM_SourcePin) != 0) {
        return RESET_REASON_PIN_RESET;
    }

    if ((reset_sources & kRCM_SourceSw) != 0) {
        return RESET_REASON_SOFTWARE;
    }

#if (defined(FSL_FEATURE_RCM_HAS_LOC) && FSL_FEATURE_RCM_HAS_LOC)
    if ((reset_sources & kRCM_SourceLoc) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_LOL) && FSL_FEATURE_RCM_HAS_LOL)
    if ((reset_sources & kRCM_SourceLol) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_JTAG) && FSL_FEATURE_RCM_HAS_JTAG)
    if ((reset_sources & kRCM_SourceJtag) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_MDM_AP) && FSL_FEATURE_RCM_HAS_MDM_AP)
    if ((reset_sources & kRCM_SourceMdmap) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

#if (defined(FSL_FEATURE_RCM_HAS_EZPORT) && FSL_FEATURE_RCM_HAS_EZPORT)
    if ((reset_sources & kRCM_SourceEzpt) != 0) {
        return RESET_REASON_PLATFORM;
    }
#endif

    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{
    return (RCM_GetPreviousResetSources(RCM) & kRCM_SourceAll);
}


void hal_reset_reason_clear(void)
{
#if (defined(FSL_FEATURE_RCM_HAS_SSRS) && FSL_FEATURE_RCM_HAS_SSRS)
    RCM_ClearStickyResetSources(RCM, kRCM_SourceAll);
#endif
}