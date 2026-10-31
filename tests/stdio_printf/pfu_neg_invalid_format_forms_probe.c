/* TC
 * id:    pfu_neg_invalid_format_forms_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (invalid formatting characters); C11 7.21.6.1 for the valid negative width/precision forms
 * note:  PROBING exactly which malformed formats the strict secure parser rejects (return -1 + handler): lone trailing '%', "%-5" (flags/width but no conversion), "%hhhd", "%!". Each is its own case. The negative '*' width/precision cases are standard-valid: negative width left-justifies, negative precision is treated as omitted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define REJ(fmt) do { char b[32]; volatile const char *f = fmt; int r; \
    tc_iph_count = 0; r = sprintf_s(b, 32, (const char *)f, 1); \
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1); } while (0)
#define WREJ(fmt) do { wchar_t b[32]; volatile const wchar_t *f = fmt; int r; \
    tc_iph_count = 0; r = swprintf_s(b, 32, (const wchar_t *)f, 1); \
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(lone_trailing_percent)   { REJ("ab%"); }
TC_CASE(wide_lone_trailing_percent) { WREJ(L"ab%"); }
TC_CASE(flags_width_no_conversion) { REJ("%-5"); }
TC_CASE(hhh_modifier)            { REJ("%hhhd"); }
TC_CASE(percent_bang)            { REJ("%!"); }
TC_CASE(negative_precision_star)
{
    char b[32]; volatile int p = -1; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, 32, "[%.*s]", p, "abc");
    CHECK_EQ_I(r, 5); CHECK_STR(b, "[abc]"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(negative_width_star)
{
    char b[32]; volatile int w = -5; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, 32, "[%*d]", w, 42);
    CHECK_EQ_I(r, 7); CHECK_STR(b, "[42   ]"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(lone_trailing_percent); TC_RUN(wide_lone_trailing_percent); TC_RUN(flags_width_no_conversion);
    TC_RUN(hhh_modifier); TC_RUN(percent_bang); TC_RUN(negative_precision_star); TC_RUN(negative_width_star);
    return tc_end();
}
