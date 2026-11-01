/* TC
 * id:    tm_wcsftime_probe_c_locale
 * area:  time
 * funcs: wcsftime
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs wcsftime (%c is locale dependent)
 * note:  PROBES that %c in the C locale is "MM/DD/YY HH:MM:SS" on Microsoft CRTs (not the ISO C "Tue Mar  9 14:05:07 2021" form).
 *        Win10 19045 ucrtbase, C locale (no setlocale call): wcsftime %c = "Tue Mar  9 14:05:07 2021" (24 chars).
 */
#include "tc.h"
#include <time.h>
#include <wchar.h>

static struct tm T;
TC_CASE(c_locale_c)
{
    wchar_t b[64];
    T.tm_year = 121; T.tm_mon = 2; T.tm_mday = 9; T.tm_hour = 14; T.tm_min = 5; T.tm_sec = 7; T.tm_wday = 2; T.tm_yday = 67;
    #if TC_CRT_VER >= 140
    /* observed on Win10 ucrtbase: %c in the "C" locale is the ISO C form (deviates from the MS-style "MM/DD/YY HH:MM:SS" of older CRTs) */
    CHECK_EQ_U(wcsftime(b, 64, L"%c", &T), 24); CHECK_WSTR(b, L"Tue Mar  9 14:05:07 2021");
#else
    CHECK_EQ_U(wcsftime(b, 64, L"%c", &T), 17); CHECK_WSTR(b, L"03/09/21 14:05:07");
#endif
}
int main(void) { tc_begin(); TC_RUN(c_locale_c); return tc_end(); }
