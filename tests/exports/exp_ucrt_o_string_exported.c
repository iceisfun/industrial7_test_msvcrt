/* TC
 * id:    exp_ucrt_o_string_exported
 * area:  exports
 * funcs: 
 * procs: _o_atoi _o_atof _o_atol _o_strtod _o_strtol _o__itoa_s _o_mbstowcs _o_wcstombs _o_qsort _o_qsort_s _o_rand _o_srand _o_rand_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   ucrtbase export table
 * note:  Undocumented _o_ conversion/stdlib aliases.
 */
#include "tc.h"

static void check_group(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) { tc_fail_expr(__FILE__, __LINE__, names[i]); }
}

static const char *const g_conversion_group[] = { "_o_atoi", "_o_atof", "_o_atol", "_o_strtod", "_o_strtol", "_o__itoa_s", "_o_mbstowcs", "_o_wcstombs" };
TC_CASE(conversion_group) { check_group(g_conversion_group, (int)(sizeof g_conversion_group / sizeof g_conversion_group[0])); }

static const char *const g_stdlib_group[] = { "_o_qsort", "_o_qsort_s", "_o_rand", "_o_srand", "_o_rand_s" };
TC_CASE(stdlib_group) { check_group(g_stdlib_group, (int)(sizeof g_stdlib_group / sizeof g_stdlib_group[0])); }

int main(void)
{
    tc_begin();
    TC_RUN(conversion_group);
    TC_RUN(stdlib_group);
    return tc_end();
}
