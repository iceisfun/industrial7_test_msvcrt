/* TC
 * id:    ct_neg_data_exports_absent_ucrt
 * area:  ctype
 * funcs:
 * procs: _pctype _pwctype _ctype __mb_cur_max __pctype_func
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs UCRT breaking changes (ctype tables are reached through functions)
 * note:  ucrtbase does not export the legacy msvcrt data symbols; headers route through __pctype_func etc.
 *        _mbctype absence is probe-grade and lives in ct_neg_mbctype_data_absent_ucrt_probe.
 */
#include "tc.h"

TC_CASE(_pctype_not_exported) { CHECK(tc_crt_proc("_pctype") == NULL); }
TC_CASE(_pwctype_not_exported) { CHECK(tc_crt_proc("_pwctype") == NULL); }
TC_CASE(_ctype_not_exported) { CHECK(tc_crt_proc("_ctype") == NULL); }
TC_CASE(__mb_cur_max_not_exported) { CHECK(tc_crt_proc("__mb_cur_max") == NULL); }
TC_CASE(__pctype_func_is_exported) { CHECK(tc_crt_proc("__pctype_func") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(_pctype_not_exported); TC_RUN(_pwctype_not_exported); TC_RUN(_ctype_not_exported);
    TC_RUN(__mb_cur_max_not_exported); TC_RUN(__pctype_func_is_exported);
    return tc_end();
}
