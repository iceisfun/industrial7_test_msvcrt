/* TC
 * id:    stl_byteswap_exports_probe
 * area:  stdlib
 * funcs:
 * procs: _byteswap_ushort _byteswap_ulong _byteswap_uint64 _rotl _rotl64
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _byteswap_*, _rotl, _rotl64 requirements
 * note:  Probes which of these intrinsic-like helpers each CRT DLL actually exports. Asserted only on ucrtbase
 *        (all non-NULL); on other CRTs the presence is recorded in a TC NOTE line and the case is skipped if absent.
 */
#include "tc.h"

static const char *const names[] = { "_byteswap_ushort", "_byteswap_ulong", "_byteswap_uint64", "_rotl", "_rotl64" };

TC_CASE(presence)
{
    int i, missing = 0;
    for (i = 0; i < 5; i++) {
        FARPROC p = tc_crt_proc(names[i]);
        tc_write("TC NOTE " TC_TEST_ID " " TC_CRT_DLL " "); tc_write(names[i]); tc_write(p ? " present\n" : " absent\n");
        if (!p) missing++;
    }
#ifdef TC_UCRT
    CHECK_EQ_I(missing, 0);
#else
    if (missing) TC_SKIP("some exports absent on this CRT (recorded)");
    CHECK_EQ_I(missing, 0);
#endif
}
int main(void) { tc_begin(); TC_RUN(presence); return tc_end(); }
