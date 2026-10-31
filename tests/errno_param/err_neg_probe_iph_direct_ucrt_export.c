/* TC
 * id:    err_neg_probe_iph_direct_ucrt_export
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: _invalid_parameter ?_invalid_parameter@@YAXPEBG00I_K@Z
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs: _invalid_parameter is an internal CRT function, not part of the documented public API
 * note:  PROBE: ucrtbase.dll exports neither plain `_invalid_parameter` nor the C++-mangled x64 `?_invalid_parameter@@YAXPEBG00I_K@Z`
 *        (only _invalid_parameter_noinfo[_noreturn] are public). The x64 mangling is checked only on _WIN64. If real Windows
 *        does export one, this test tells us, and err_neg_iph_direct_* can then be extended to ucrtbase.
 */
#include "tc.h"

TC_CASE(plain_not_exported)   { CHECK(tc_crt_proc("_invalid_parameter") == NULL); }
#ifdef _WIN64
TC_CASE(mangled_not_exported) { CHECK(tc_crt_proc("?_invalid_parameter@@YAXPEBG00I_K@Z") == NULL); }
#endif
TC_CASE(noinfo_exported)      { CHECK(tc_crt_proc("_invalid_parameter_noinfo") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(plain_not_exported);
#ifdef _WIN64
    TC_RUN(mangled_not_exported);
#endif
    TC_RUN(noinfo_exported);
    return tc_end();
}
