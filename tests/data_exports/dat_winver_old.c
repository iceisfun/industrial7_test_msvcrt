/* TC
 * id:    dat_winver_old
 * area:  data_exports
 * funcs:
 * procs: _winver _winmajor _winminor
 * crt:   msvcr80 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _winmajor, _winminor, _winver, _osver (global variables, stdlib.h)
 * note:  Data exports are resolved with GetProcAddress (address of the variable). Compared with kernel32 GetVersion().
 */
#include "tc.h"

static volatile unsigned int *p_winver, *p_winmajor, *p_winminor;

static DWORD ver(void) { return GetVersion(); }

TC_CASE(winmajor_matches_getversion)
{
    if (!p_winmajor) TC_SKIP("_winmajor not exported");
    CHECK_EQ_U(*p_winmajor, ver() & 0xff);
}
TC_CASE(winminor_matches_getversion)
{
    if (!p_winminor) TC_SKIP("_winminor not exported");
    CHECK_EQ_U(*p_winminor, (ver() >> 8) & 0xff);
}
TC_CASE(winver_is_major_shl8_plus_minor)
{
    if (!p_winver || !p_winmajor || !p_winminor) TC_SKIP("not exported");
    CHECK_EQ_U(*p_winver, (*p_winmajor << 8) + *p_winminor);
}
int main(void)
{
    tc_begin();
    p_winver = (volatile unsigned int *)tc_crt_proc("_winver");
    p_winmajor = (volatile unsigned int *)tc_crt_proc("_winmajor");
    p_winminor = (volatile unsigned int *)tc_crt_proc("_winminor");
    TC_RUN(winmajor_matches_getversion); TC_RUN(winminor_matches_getversion); TC_RUN(winver_is_major_shl8_plus_minor);
    return tc_end();
}
