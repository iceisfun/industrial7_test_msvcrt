/* TC
 * id:    exp_neg_getprocaddress_ordinal_invalid
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs GetProcAddress (returns NULL on failure; GetLastError gives the reason); ERROR_PROC_NOT_FOUND is the observed code
 * note:  Ordinal 0xFFFF (no CRT has that many exports) and absurd/odd names must fail with NULL and ERROR_PROC_NOT_FOUND (observed, not documented). Ordinal 0 is deliberately not tested: GetProcAddress(h, NULL) is not a documented input.
 * OBSERVED (Win10 22H2 19045; raw "GetLastError() == ERROR_PROC_NOT_FOUND got=182 want=127"): an out-of-range ORDINAL gives
 *        ERROR_INVALID_ORDINAL (182); only absent NAMES give ERROR_PROC_NOT_FOUND (127). Class A. Stable (loader behaviour).
 */
#include "tc.h"
#include <stddef.h>

static HMODULE crt;

TC_CASE(ordinal_ffff)
{
    FARPROC p; SetLastError(0);
    p = GetProcAddress(crt, MAKEINTRESOURCEA(0xFFFF));
    CHECK(p == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_INVALID_ORDINAL);   /* observed Win10 22H2: 182, not 127 */
}
TC_CASE(long_name)
{
    static char n[4001]; FARPROC p; int i;
    for (i = 0; i < 4000; i++) n[i] = 'a';
    n[4000] = 0;
    SetLastError(0);
    p = GetProcAddress(crt, n);
    CHECK(p == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_PROC_NOT_FOUND);
}
TC_CASE(control_suffix)
{
    CHECK(GetProcAddress(crt, "malloc\t") == NULL);
    CHECK(GetProcAddress(crt, "malloc\n") == NULL);
    CHECK(GetProcAddress(crt, "malloc ") == NULL);
    CHECK(GetProcAddress(crt, "") == NULL);
}
TC_CASE(control_positive)
{
    CHECK(GetProcAddress(crt, "malloc") != NULL);
}
int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(ordinal_ffff); TC_RUN(long_name);
    TC_RUN(control_suffix); TC_RUN(control_positive);
    return tc_end();
}
