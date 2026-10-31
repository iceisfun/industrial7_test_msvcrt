/* TC
 * id:    dat_osver_get_probe
 * area:  data_exports
 * funcs:
 * procs: _get_osplatform _get_osver _get_winmajor _get_winminor
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_osplatform, _get_osver, _get_winmajor, _get_winminor (errno_t f(unsigned int *); EINVAL on NULL)
 * note:  PROBE: unsure which of these CRTs export the getters; a missing export is reported as SKIP, not FAIL. When present,
 *        values must match GetVersion() and NULL must give EINVAL (return value, via the invalid parameter handler).
 *        A match of osplatform with VER_PLATFORM_WIN32_NT (2) is probed too.
 */
#include "tc.h"

typedef int (__cdecl *get_t)(unsigned int *);
static get_t g_plat, g_osver, g_major, g_minor;

TC_CASE(values_match_GetVersion)
{
    unsigned int v = 0xdeadbeef; DWORD gv = GetVersion();
    if (!g_major || !g_minor) TC_SKIP("getters not exported");
    CHECK_EQ_I(g_major(&v), 0); CHECK_EQ_U(v, gv & 0xff);
    CHECK_EQ_I(g_minor(&v), 0); CHECK_EQ_U(v, (gv >> 8) & 0xff);
}
TC_CASE(osplatform_is_nt)
{
    unsigned int v = 0;
    if (!g_plat) TC_SKIP("_get_osplatform not exported");
    CHECK_EQ_I(g_plat(&v), 0); CHECK_EQ_U(v, 2);
}
TC_CASE(osver_is_build)
{
    unsigned int v = 0; DWORD gv = GetVersion();
    if (!g_osver) TC_SKIP("_get_osver not exported");
    CHECK_EQ_I(g_osver(&v), 0); CHECK_EQ_U(v & 0x7fff, (gv >> 16) & 0x7fff);
}
TC_CASE(null_arg_einval)
{
    if (!g_major || !g_minor || !tc_iph_install()) TC_SKIP("getters not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(g_major(NULL), 22);
    CHECK_EQ_I(g_minor(NULL), 22);
    CHECK(tc_iph_count == 2);
}
int main(void)
{
    tc_begin();
    g_plat = (get_t)tc_crt_proc("_get_osplatform"); g_osver = (get_t)tc_crt_proc("_get_osver");
    g_major = (get_t)tc_crt_proc("_get_winmajor"); g_minor = (get_t)tc_crt_proc("_get_winminor");
    TC_RUN(values_match_GetVersion); TC_RUN(osplatform_is_nt); TC_RUN(osver_is_build); TC_RUN(null_arg_einval);
    return tc_end();
}
