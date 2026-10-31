/* TC
 * id:    pfo_neg_printf_count_output_effects
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf fprintf _snprintf
 * procs: _set_printf_count_output _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output: with %n support disabled (default), the printf family treats %n as invalid and calls the invalid parameter handler
 * note:  msvcr80 excluded (import-library gap). The flag is restored to 0 at the end. The temp file is removed.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(sprintf_n)
{
    char b[32]; int n = 0x5a5a5a5a, r;
    tc_p__set_printf_count_output(0); tc_iph_count = 0;
    r = sprintf(b, "ab%n", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fprintf_n)
{
    const char *name = "tc_pfo_neg_pco_effects.tmp";
    FILE *f; int n = 0x5a5a5a5a, r;
    tc_p__set_printf_count_output(0); tc_iph_count = 0;
    f = fopen(name, "w");
    CHECK(f != NULL);
    if (!f) return;
    r = fprintf(f, "ab%n", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
    remove(name);
}
TC_CASE(snprintf_n)
{
    char b[32]; int n = 0x5a5a5a5a, r;
    tc_p__set_printf_count_output(0); tc_iph_count = 0;
    r = _snprintf(b, sizeof b, "ab%n", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_n); TC_RUN(fprintf_n); TC_RUN(snprintf_n);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
