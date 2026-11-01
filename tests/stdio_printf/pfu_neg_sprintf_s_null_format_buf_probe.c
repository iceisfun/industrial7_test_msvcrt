/* TC
 * id:    pfu_neg_sprintf_s_null_format_buf_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (silent about buffer state on NULL format)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): with a NULL format the buffer is NOT touched (b[0] stays 'Z'); return -1 and the handler runs once. The old guess (b[0]==0) was wrong:
 *        the UCRT validates the format before writing anything.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(narrow)
{
    char b[10] = "ZZZ"; const char *volatile f = NULL; int n;
    tc_iph_count = 0;
    n = sprintf_s(b, 10, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide)
{
    wchar_t b[10] = L"ZZZ"; const wchar_t *volatile f = NULL; int n;
    tc_iph_count = 0;
    n = swprintf_s(b, 10, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], L'Z'); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    TC_RUN(narrow); TC_RUN(wide);
    return tc_end();
}
