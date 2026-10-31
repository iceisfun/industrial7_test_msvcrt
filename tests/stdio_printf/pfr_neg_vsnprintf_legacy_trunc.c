/* TC
 * id:    pfr_neg_vsnprintf_legacy_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf: len > count -> count bytes written, no NUL, returns -1; len == count -> returns count, no NUL
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define G 0x5A
static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(count_lt_len)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = cv(b, 3, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c');
    CHECK_EQ_I((unsigned char)b[3], G);
}
TC_CASE(len_eq_count_plus_1)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = cv(b, 5, "%s", "abcdef");   /* len 6 == count+1 */
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[4], 'e');
    CHECK_EQ_I((unsigned char)b[5], G);
}
TC_CASE(exact_fit_no_nul)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = cv(b, 6, "%s", "abcdef");
    CHECK_EQ_I(r, 6);
    CHECK_EQ_I(b[5], 'f');
    CHECK_EQ_I((unsigned char)b[6], G);
}
TC_CASE(count_plus_one_gets_nul)
{
    char b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = cv(b, 7, "%s", "abcdef");
    CHECK_EQ_I(r, 6);
    CHECK_STR(b, "abcdef");
}
int main(void)
{
    tc_begin();
    TC_RUN(count_lt_len); TC_RUN(len_eq_count_plus_1); TC_RUN(exact_fit_no_nul); TC_RUN(count_plus_one_gets_nul);
    return tc_end();
}
