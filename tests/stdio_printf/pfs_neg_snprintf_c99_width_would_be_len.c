/* TC
 * id:    pfs_neg_snprintf_c99_width_would_be_len
 * area:  stdio_printf
 * funcs: snprintf _scprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf (returns the length that would have been written, output truncated and NUL-terminated); MS docs _scprintf
 * note:  Width padding is counted in the would-be length but only size-1 characters plus NUL are stored.
 */
#include "tc.h"
#include <stdio.h>

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static const char *volatile f_10s = "%10s";
static const char *volatile f_m10s = "%-10s|";
static const char *volatile f_c = "%c";
static const char *volatile f_star = "%*s";
static volatile int hundred = 100;
static volatile int xc = 'x';

TC_CASE(right_padded)
{
    char b[16]; int n; fill(b);
    n = snprintf(b, 4, f_10s, "ab");
    CHECK_EQ_I(n, 10); CHECK_STR(b, "   "); CHECK(b[3] == 0); CHECK(b[4] == 'Z');
}
TC_CASE(char_size1)
{
    char b[16]; int n; fill(b);
    n = snprintf(b, 1, f_c, xc);
    CHECK_EQ_I(n, 1); CHECK(b[0] == 0); CHECK(b[1] == 'Z');
}
TC_CASE(left_padded)
{
    char b[16]; int n; fill(b);
    n = snprintf(b, 4, f_m10s, "ab");
    CHECK_EQ_I(n, 11); CHECK_STR(b, "ab "); CHECK(b[4] == 'Z');
}
TC_CASE(scprintf_star)
{
    int n = _scprintf(f_star, hundred, "x");
    CHECK_EQ_I(n, 100);
}
int main(void)
{
    tc_begin();
    TC_RUN(right_padded); TC_RUN(char_size1); TC_RUN(left_padded); TC_RUN(scprintf_star);
    return tc_end();
}
