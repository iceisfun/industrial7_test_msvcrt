/* TC
 * id:    pfm_2_neg_snprintf_c99_trunc_ucrt
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf (returns would-be length; n==0 allows NULL buffer; output beyond n-1 discarded, NUL written at n-1); MS docs snprintf (UCRT)
 */
#include "tc.h"
#include <stdio.h>

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Q'; }

TC_CASE(n_lt_len_returns_full_len_with_nul)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = snprintf(b, 3, f, "abcde");
    CHECK_EQ_I(n, 5); CHECK_MEM(b, "ab\0Q", 4);
}
TC_CASE(n_eq_len_truncates_last)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = snprintf(b, 5, f, "abcde");
    CHECK_EQ_I(n, 5); CHECK_MEM(b, "abcd\0Q", 6);
}
TC_CASE(n_zero_null_buffer)
{
    const char * volatile f = "%d-%s"; char * volatile nb = NULL;
    CHECK_EQ_I(snprintf(nb, 0, f, 123, "xy"), 6);
}
TC_CASE(n_zero_buffer_untouched)
{
    char b[16]; const char * volatile f = "%s"; int n; fill(b);
    n = snprintf(b, 0, f, "abcde");
    CHECK_EQ_I(n, 5); CHECK_MEM(b, "QQQQQQ", 6);
}
int main(void) { tc_begin(); TC_RUN(n_lt_len_returns_full_len_with_nul); TC_RUN(n_eq_len_truncates_last); TC_RUN(n_zero_null_buffer); TC_RUN(n_zero_buffer_untouched); return tc_end(); }
