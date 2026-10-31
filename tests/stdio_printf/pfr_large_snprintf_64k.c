/* TC
 * id:    pfr_large_snprintf_64k
 * area:  stdio_printf
 * funcs: _snprintf _scprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf (returns -1 if the output would exceed count; NUL appended only if there is room), _scprintf
 * note:  Outputs of 65535/65536/70000 chars, heap-allocated buffers.
 */
#include "tc.h"
#include <stdio.h>

static char *mk(size_t n) { return (char *)HeapAlloc(GetProcessHeap(), 0, n); }
static void fr(void *p) { HeapFree(GetProcessHeap(), 0, p); }
static void pat(char *s, size_t n) { size_t i; for (i = 0; i < n; i++) s[i] = (char)('a' + i % 26); s[n] = 0; }

TC_CASE(scprintf_70000)
{
    volatile int w = 70000;
    CHECK_EQ_I(_scprintf("%*d", w, 1), 70000);
    w = 65535; CHECK_EQ_I(_scprintf("%*d", w, 1), 65535);
    w = 65536; CHECK_EQ_I(_scprintf("%*d", w, 1), 65536);
}
TC_CASE(fits_with_nul)
{
    char *src = mk(70001), *dst = mk(70001 + 8); int r; size_t i, ok = 1;
    CHECK(src && dst); if (!src || !dst) { fr(src); fr(dst); return; }
    pat(src, 70000);
    dst[70001] = 0x7e;
    r = _snprintf(dst, 70001, "%s", src);
    CHECK_EQ_I(r, 70000);
    CHECK_EQ_I(dst[70000], 0);
    for (i = 0; i < 70000; i++) if (dst[i] != src[i]) { ok = 0; break; }
    CHECK(ok);
    CHECK_EQ_I(dst[70001], 0x7e);
    fr(src); fr(dst);
}
TC_CASE(zero_pad_width_65536)
{
    char *d = mk(65536 + 8); volatile int w = 65536; int r, i, ok = 1;
    CHECK(d != NULL); if (!d) return;
    r = _snprintf(d, 65536 + 8, "%0*d", w, 987);
    CHECK_EQ_I(r, 65536);
    CHECK_EQ_I(d[65536], 0);
    for (i = 0; i < 65533; i++) if (d[i] != '0') { ok = 0; break; }
    CHECK(ok);
    CHECK(d[65533] == '9' && d[65534] == '8' && d[65535] == '7');
    fr(d);
}
TC_CASE(exact_fit_no_nul)
{
    char *d = mk(65536 + 8), *s = mk(65537); int r;
    CHECK(d && s); if (!d || !s) { fr(d); fr(s); return; }
    pat(s, 65536);
    d[65536] = 0x7e;
    r = _snprintf(d, 65536, "%s", s);
    CHECK_EQ_I(r, 65536);
    CHECK_EQ_I(d[65535], s[65535]);
    CHECK_EQ_I(d[65536], 0x7e);   /* no terminator written */
    fr(d); fr(s);
}
TC_CASE(neg_trunc_70000_into_65536)
{
    char *d = mk(65536 + 8), *s = mk(70001); int r; size_t i, ok = 1;
    CHECK(d && s); if (!d || !s) { fr(d); fr(s); return; }
    pat(s, 70000);
    d[65536] = 0x7e;
    r = _snprintf(d, 65536, "%s", s);
    CHECK_EQ_I(r, -1);
    for (i = 0; i < 65536; i++) if (d[i] != s[i]) { ok = 0; break; }
    CHECK(ok);
    CHECK_EQ_I(d[65536], 0x7e);
    fr(d); fr(s);
}
TC_CASE(neg_trunc_65535_one_over)
{
    char *d = mk(65535 + 8), *s = mk(65537); int r;
    CHECK(d && s); if (!d || !s) { fr(d); fr(s); return; }
    pat(s, 65536);
    d[65535] = 0x7e;
    r = _snprintf(d, 65535, "%s", s);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(d[65535], 0x7e);
    fr(d); fr(s);
}
int main(void)
{
    tc_begin();
    TC_RUN(scprintf_70000); TC_RUN(fits_with_nul); TC_RUN(zero_pad_width_65536);
    TC_RUN(exact_fit_no_nul); TC_RUN(neg_trunc_70000_into_65536); TC_RUN(neg_trunc_65535_one_over);
    return tc_end();
}
