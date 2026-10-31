/* TC
 * id:    exp_ucrt_o_math_exported
 * area:  exports
 * funcs: 
 * procs: _o_sin _o_cos _o_tan _o_exp _o_log _o_pow _o_floor _o_ceil _o_acoshl
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   ucrtbase export table
 * note:  Undocumented _o_ math aliases.
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_double_group[] = { "_o_sin", "_o_cos", "_o_tan", "_o_exp", "_o_log", "_o_pow", "_o_floor", "_o_ceil" };
TC_CASE(double_group) { check_group(g_double_group, (int)(sizeof g_double_group / sizeof g_double_group[0])); }

static const char *const g_long_double_alias[] = { "_o_acoshl" };
TC_CASE(long_double_alias) { check_group(g_long_double_alias, (int)(sizeof g_long_double_alias / sizeof g_long_double_alias[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(double_group);
    TC_RUN(long_double_alias);
    return tc_end();
}
