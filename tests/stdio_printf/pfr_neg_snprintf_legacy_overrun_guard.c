/* TC
 * id:    pfr_neg_snprintf_legacy_overrun_guard
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf: at most count bytes are ever written
 */
#include "tc.h"
#include <stdio.h>

#define G 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = G; }
static int guard(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }

TC_CASE(percent_s)  { char b[64]; fill(b, 64); CHECK_EQ_I(_snprintf(b, 8, "%s", "0123456789abcdef"), -1); CHECK(guard(b, 8, 64)); }
TC_CASE(percent_d)  { char b[64]; fill(b, 64); CHECK_EQ_I(_snprintf(b, 4, "%d", 1234567), -1); CHECK(guard(b, 4, 64)); }
TC_CASE(percent_f)  { char b[64]; fill(b, 64); CHECK_EQ_I(_snprintf(b, 5, "%f", 1234.5), -1); CHECK(guard(b, 5, 64)); }
TC_CASE(pad_s)      { char b[64]; fill(b, 64); CHECK_EQ_I(_snprintf(b, 6, "%10s", "ab"), -1); CHECK(guard(b, 6, 64));
                      CHECK(b[0] == ' ' && b[5] == ' '); }
TC_CASE(pad_100d)
{
    char b[64]; volatile int w = 100; fill(b, 64);
    CHECK_EQ_I(_snprintf(b, 8, "%*d", w, 5), -1);
    CHECK(guard(b, 8, 64));
    CHECK(b[0] == ' ' && b[7] == ' ');
}
TC_CASE(long_multi_arg)
{
    char b[64]; fill(b, 64);
    CHECK_EQ_I(_snprintf(b, 10, "%s-%d-%s-%d", "aaaa", 1, "bbbb", 2), -1);
    CHECK_EQ_I(b[9], 'b'); CHECK(guard(b, 10, 64));
}
int main(void)
{
    tc_begin();
    TC_RUN(percent_s); TC_RUN(percent_d); TC_RUN(percent_f); TC_RUN(pad_s); TC_RUN(pad_100d); TC_RUN(long_multi_arg);
    return tc_end();
}
