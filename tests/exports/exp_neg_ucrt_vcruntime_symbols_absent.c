/* TC
 * id:    exp_neg_ucrt_vcruntime_symbols_absent
 * area:  exports
 * funcs:
 * procs: _CxxThrowException __CxxFrameHandler3 __C_specific_handler
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   VCRuntime/UCRT split (VS2015)
 * note:  Win10 22H2 ucrtbase export table (observed via GetProcAddress, real non-forwarded exports): _CxxThrowException, __CxxFrameHandler3 and
 *        __C_specific_handler ARE exported by ucrtbase.dll. The old probe claimed they only live in vcruntime140. (Test id keeps its historical "_absent" suffix.)
 */
#include "tc.h"

static void chk(const char *const *n, int cnt)
{
    int i; for (i = 0; i < cnt; i++) CHECK(tc_crt_proc(n[i]) != NULL);
}
TC_CASE(cxx_throw_exported) { static const char *const n[] = {"_CxxThrowException"}; chk(n, 1); }
TC_CASE(frame_handler_exported) { static const char *const n[] = {"__CxxFrameHandler3"}; chk(n, 1); }
TC_CASE(c_specific_handler_exported) { static const char *const n[] = {"__C_specific_handler"}; chk(n, 1); }

int main(void)
{
    tc_begin();
    TC_RUN(cxx_throw_exported);
    TC_RUN(frame_handler_exported);
    TC_RUN(c_specific_handler_exported);
    return tc_end();
}
