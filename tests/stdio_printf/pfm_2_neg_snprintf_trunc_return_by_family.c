/* TC
 * id:    pfm_2_neg_snprintf_trunc_return_by_family
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _vsnprintf _snprintf_s
 * crt:   >=120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _vsnprintf ("If len < count then len chars stored and NUL appended; len == count: len chars, no NUL; len > count: count chars stored, no NUL, -1 returned"); _snprintf_s (_TRUNCATE)
 * note:  Truncation return value and NUL-termination matrix for the legacy and _s families. errno / default-handler details are in pfm_2_neg_snprintf_s_errno_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vsn(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Q'; }

TC_CASE(snprintf_n_eq_len_no_nul)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = _snprintf(b, 5, f, "abcde");
    CHECK_EQ_I(n, 5); CHECK_MEM(b, "abcdeQ", 6);
}
TC_CASE(snprintf_n_lt_len_minus1_no_nul)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = _snprintf(b, 3, f, "abcde");
    CHECK_EQ_I(n, -1); CHECK_MEM(b, "abcQ", 4);
}
TC_CASE(snprintf_n_gt_len_nul)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = _snprintf(b, 6, f, "abcde");
    CHECK_EQ_I(n, 5); CHECK_STR(b, "abcde");
}
TC_CASE(vsnprintf_same_matrix)
{
    char b[16]; int n;
    fill(b); n = vsn(b, 5, "%s", "abcde");  CHECK_EQ_I(n, 5);  CHECK_MEM(b, "abcdeQ", 6);
    fill(b); n = vsn(b, 4, "%s", "abcde");  CHECK_EQ_I(n, -1); CHECK_MEM(b, "abcdQ", 5);
    fill(b); n = vsn(b, 6, "%s", "abcde");  CHECK_EQ_I(n, 5);  CHECK_STR(b, "abcde");
}
TC_CASE(snprintf_s_truncate)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = _snprintf_s(b, 4, _TRUNCATE, f, "abcde");
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc");
}
TC_CASE(snprintf_s_count_limits_output)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = _snprintf_s(b, 16, 3, f, "abcde");   /* count < size: first 3 chars, NUL appended, -1 */
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc");
}
TC_CASE(snprintf_s_fits)
{
    char b[16]; const char * volatile f = "%d"; int n; fill(b);
    n = _snprintf_s(b, 4, _TRUNCATE, f, 123);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "123");
}
int main(void)
{
    tc_begin();
    TC_RUN(snprintf_n_eq_len_no_nul); TC_RUN(snprintf_n_lt_len_minus1_no_nul); TC_RUN(snprintf_n_gt_len_nul);
    TC_RUN(vsnprintf_same_matrix); TC_RUN(snprintf_s_truncate); TC_RUN(snprintf_s_count_limits_output); TC_RUN(snprintf_s_fits);
    return tc_end();
}
