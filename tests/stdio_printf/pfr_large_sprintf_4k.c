/* TC
 * id:    pfr_large_sprintf_4k
 * area:  stdio_printf
 * funcs: sprintf _scprintf
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.6 (sprintf returns chars written excl NUL); MS docs sprintf, _scprintf
 * note:  Outputs crossing the 4096-byte boundary; no internal buffer limit may truncate or corrupt.
 */
#include "tc.h"
#include <stdio.h>

static char *mk(size_t n) { char *p = (char *)HeapAlloc(GetProcessHeap(), 0, n); return p; }
static void fr(void *p) { HeapFree(GetProcessHeap(), 0, p); }
static void pat(char *s, size_t n) { size_t i; for (i = 0; i < n; i++) s[i] = (char)('a' + i % 26); s[n] = 0; }

static void one_len(size_t n)
{
    char *src = mk(n + 1), *dst = mk(n + 64); int r; size_t i, ok = 1;
    CHECK(src && dst);
    if (!src || !dst) { fr(src); fr(dst); return; }
    pat(src, n);
    dst[n + 1] = 0x7e;
    r = sprintf(dst, "%s", src);
    CHECK_EQ_I(r, n);
    for (i = 0; i < n; i++) if (dst[i] != src[i]) { ok = 0; break; }
    CHECK(ok);
    CHECK_EQ_I(dst[n], 0);
    CHECK_EQ_I(dst[n + 1], 0x7e);
    fr(src); fr(dst);
}
TC_CASE(s_4095) { one_len(4095); }
TC_CASE(s_4096) { one_len(4096); }
TC_CASE(s_4097) { one_len(4097); }
TC_CASE(pad_5000)
{
    char *d = mk(5100); volatile int w = 5000; int r, i, ok = 1;
    CHECK(d != NULL); if (!d) return;
    r = sprintf(d, "%*d", w, 12345);
    CHECK_EQ_I(r, 5000);
    for (i = 0; i < 4995; i++) if (d[i] != ' ') { ok = 0; break; }
    CHECK(ok);
    CHECK_STR(d + 4995, "12345");
    fr(d);
}
TC_CASE(prefix_plus_pad)
{
    char *d = mk(5100); int r;
    CHECK(d != NULL); if (!d) return;
    r = sprintf(d, "ab%5000dcd", 1);
    CHECK_EQ_I(r, 5004);
    CHECK(d[0] == 'a' && d[1] == 'b' && d[5001] == '1' && d[5002] == 'c' && d[5003] == 'd' && d[5004] == 0);
    fr(d);
}
TC_CASE(scprintf_widths)
{
    volatile int w;
    w = 4095; CHECK_EQ_I(_scprintf("%*d", w, 1), 4095);
    w = 4096; CHECK_EQ_I(_scprintf("%*d", w, 1), 4096);
    w = 4097; CHECK_EQ_I(_scprintf("%*d", w, 1), 4097);
    w = 5000; CHECK_EQ_I(_scprintf("%*d", w, 1), 5000);
}
TC_CASE(scprintf_agrees_sprintf)
{
    char *d = mk(5100); volatile int w = 4100; int a, b;
    CHECK(d != NULL); if (!d) return;
    a = sprintf(d, "[%-*s]", w, "x");
    b = _scprintf("[%-*s]", w, "x");
    CHECK_EQ_I(a, 4102);
    CHECK_EQ_I(b, a);
    fr(d);
}
int main(void)
{
    tc_begin();
    TC_RUN(s_4095); TC_RUN(s_4096); TC_RUN(s_4097); TC_RUN(pad_5000);
    TC_RUN(prefix_plus_pad); TC_RUN(scprintf_widths); TC_RUN(scprintf_agrees_sprintf);
    return tc_end();
}
