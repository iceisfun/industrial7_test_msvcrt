/* TC
 * id:    exp_neg_ucrt_vcruntime_symbols_absent
 * area:  exports
 * funcs:
 * procs: _CxxThrowException __CxxFrameHandler3 __C_specific_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   VCRuntime/UCRT split (VS2015)
 * note:  Probing: C++/SEH runtime symbols live in vcruntime140.dll and are NOT exported by ucrtbase.dll (mingw import lib lists them but is not authoritative).
 */
#include "tc.h"

static void chk(const char *const *n, int cnt)
{
    int i; for (i = 0; i < cnt; i++) CHECK(tc_crt_proc(n[i]) == NULL);
}
TC_CASE(cxx_throw_absent) { static const char *const n[] = {"_CxxThrowException"}; chk(n, 1); }
TC_CASE(frame_handler_absent) { static const char *const n[] = {"__CxxFrameHandler3"}; chk(n, 1); }
TC_CASE(c_specific_handler_absent) { static const char *const n[] = {"__C_specific_handler"}; chk(n, 1); }

int main(void)
{
    tc_begin();
    TC_RUN(cxx_throw_absent);
    TC_RUN(frame_handler_absent);
    TC_RUN(c_specific_handler_absent);
    return tc_end();
}
