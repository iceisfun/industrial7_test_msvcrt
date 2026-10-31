/* TC
 * id:    pfl_probe_sprintf_l_apostrophe_flag
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf format specification (flags: - + 0 space #; no grouping flag)
 * note:  PROBE: the POSIX apostrophe grouping flag is not a Microsoft flag. Expectation: ucrt treats "%'d" as an invalid format
 *        (invalid-parameter handler called, return -1, errno EINVAL). Only the return value, errno and handler count are asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static volatile int v_i = 1234567;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

static void run(const char *name)
{
    char b[64]; int n; _locale_t l = _create_locale(LC_ALL, name);
    if (!l) { TC_SKIP("locale missing"); }
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    b[0] = 0;
    n = _sprintf_l(b, "%'d", l, v_i);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    _free_locale(l);
}
TC_CASE(apostrophe_d_c_locale_result_or_iph) { run(v_c); }
TC_CASE(apostrophe_d_de_locale_result_or_iph) { run(v_de); }

int main(void) { tc_begin(); TC_RUN(apostrophe_d_c_locale_result_or_iph); TC_RUN(apostrophe_d_de_locale_result_or_iph); return tc_end(); }
