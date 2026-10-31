/* TC
 * id:    dat_osver_build_probe
 * area:  data_exports
 * funcs:
 * procs: _osver
 * crt:   msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _osver (build number of the OS)
 * note:  PROBE: _osver is believed to equal (GetVersion() >> 16) & 0x7fff on NT-family Windows (high bit set only on Win9x).
 *        Same source as GetVersion, so manifest compat shimming should not matter. Not stated verbatim in docs.
 */
#include "tc.h"

static volatile unsigned int *p_osver;

TC_CASE(osver_equals_getversion_build)
{
    DWORD v = GetVersion();
    if (!p_osver) TC_SKIP("_osver not exported");
    CHECK_EQ_U(*p_osver & 0x7fff, (v >> 16) & 0x7fff);
}
TC_CASE(osver_high_bit_clear_nt)
{
    DWORD v = GetVersion();
    if (!p_osver) TC_SKIP("_osver not exported");
    CHECK(!(v & 0x80000000u));
    CHECK_EQ_U(*p_osver & 0x8000, 0);
}
int main(void)
{
    tc_begin();
    p_osver = (volatile unsigned int *)tc_crt_proc("_osver");
    TC_RUN(osver_equals_getversion_build); TC_RUN(osver_high_bit_clear_nt);
    return tc_end();
}
