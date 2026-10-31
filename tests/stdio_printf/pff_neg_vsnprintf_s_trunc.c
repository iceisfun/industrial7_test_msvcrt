/* TC
 * id:    pff_neg_vsnprintf_s_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_s, _vsnprintf_s: count < size truncates to count chars and returns -1; _TRUNCATE fills size-1
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int w(char *b, size_t sz, size_t cnt, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, sz, cnt, f, ap); va_end(ap); return r; }

TC_CASE(truncate_flag)
{
    char b[32]; volatile size_t sz = 6; int r, i;
    for (i = 0; i < 32; i++) b[i] = 0x5A;
    tc_iph_count = 0;
    r = w(b, sz, _TRUNCATE, "%f", 1.5);
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "1.500");
    CHECK_EQ_I(b[6], 0x5A);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_below_size)
{
    char b[32]; volatile size_t cnt = 3; int r, i;
    for (i = 0; i < 32; i++) b[i] = 0x5A;
    tc_iph_count = 0;
    r = w(b, 16, cnt, "%f", 1.5);
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "1.5");
    CHECK_EQ_I(b[4], 0x5A);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(truncate_flag); TC_RUN(count_below_size); return tc_end();
}
