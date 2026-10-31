/* TC
 * id:    exp_msvcr_cxx_runtime_exported
 * area:  exports
 * funcs: 
 * procs: _CxxThrowException
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  observed
 * ref:   MSVC C++ EH ABI; x64 SEH (__C_specific_handler)
 * note:  In UCRT these moved to vcruntime140. msvcrt.dll excluded (unsure).  __C_specific_handler moved to its own probe test (exp_msvcr_c_specific_handler_probe).
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_cxxthrow_exported[] = { "_CxxThrowException" };
TC_CASE(cxxthrow_exported) { check_group(g_cxxthrow_exported, (int)(sizeof g_cxxthrow_exported / sizeof g_cxxthrow_exported[0])); }


int main(void)
{
    tc_begin();
    TC_RUN(cxxthrow_exported);
    return tc_end();
}
