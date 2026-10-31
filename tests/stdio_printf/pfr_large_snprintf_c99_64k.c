/* TC
 * id:    pfr_large_snprintf_c99_64k
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf; MS docs snprintf (UCRT is C99-conforming: returns would-be length)
 */
#include "tc.h"
#include <stdio.h>

static char *mk(size_t n) { return (char *)HeapAlloc(GetProcessHeap(), 0, n); }
static void fr(void *p) { HeapFree(GetProcessHeap(), 0, p); }

TC_CASE(null_zero)
{
    volatile int w = 70000;
    CHECK_EQ_I(snprintf(NULL, 0, "%*d", w, 1), 70000);
}
TC_CASE(small_buffer_would_be)
{
    char b[104]; volatile int w = 70000; int r, i;
    for (i = 0; i < 104; i++) b[i] = 0x5a;
    r = snprintf(b, 100, "%*d", w, 1);
    CHECK_EQ_I(r, 70000);
    for (i = 0; i < 99; i++) if (b[i] != ' ') { CHECK_EQ_I(i, -1); break; }
    CHECK_EQ_I(b[99], 0);
    CHECK_EQ_I(b[100], 0x5a);
}
TC_CASE(precision_100000_string)
{
    char *s = mk(70001), *d = mk(70010); int r, i, ok = 1; volatile int p = 100000;
    CHECK(s && d); if (!s || !d) { fr(s); fr(d); return; }
    for (i = 0; i < 70000; i++) s[i] = (char)('a' + i % 26);
    s[70000] = 0;
    r = snprintf(d, 70010, "%.*s", p, s);
    CHECK_EQ_I(r, 70000);
    for (i = 0; i < 70000; i++) if (d[i] != s[i]) { ok = 0; break; }
    CHECK(ok);
    CHECK_EQ_I(d[70000], 0);
    fr(s); fr(d);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_zero); TC_RUN(small_buffer_would_be); TC_RUN(precision_100000_string);
    return tc_end();
}
