/* TC
 * id:    pfr_large_wide_4k
 * area:  stdio_printf
 * funcs: _snwprintf _scwprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf (-1 on overflow of count), _scprintf, _scwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static wchar_t *mk(size_t n) { return (wchar_t *)HeapAlloc(GetProcessHeap(), 0, n * sizeof(wchar_t)); }
static void fr(void *p) { HeapFree(GetProcessHeap(), 0, p); }
static void pat(wchar_t *s, size_t n) { size_t i; for (i = 0; i < n; i++) s[i] = (wchar_t)(L'a' + i % 26); s[n] = 0; }

TC_CASE(scwprintf_5000)
{
    volatile int w = 5000;
    CHECK_EQ_I(_scwprintf(L"%*d", w, 7), 5000);
}
TC_CASE(snwprintf_70000)
{
    wchar_t *s = mk(70001), *d = mk(70010); int r; size_t i, ok = 1;
    CHECK(s && d); if (!s || !d) { fr(s); fr(d); return; }
    pat(s, 70000);
    d[70001] = 0x7e;
    r = _snwprintf(d, 70001, L"%ls", s);
    CHECK_EQ_I(r, 70000);
    CHECK_EQ_I(d[70000], 0);
    for (i = 0; i < 70000; i++) if (d[i] != s[i]) { ok = 0; break; }
    CHECK(ok);
    CHECK_EQ_I(d[70001], 0x7e);
    fr(s); fr(d);
}
TC_CASE(neg_trunc_4096)
{
    wchar_t *s = mk(5001), *d = mk(4200); int r; size_t i, ok = 1;
    CHECK(s && d); if (!s || !d) { fr(s); fr(d); return; }
    pat(s, 5000);
    d[4096] = 0x7e;
    r = _snwprintf(d, 4096, L"%ls", s);
    CHECK_EQ_I(r, -1);
    for (i = 0; i < 4096; i++) if (d[i] != s[i]) { ok = 0; break; }
    CHECK(ok);
    CHECK_EQ_I(d[4096], 0x7e);
    fr(s); fr(d);
}
TC_CASE(exact_4096_no_nul)
{
    wchar_t *s = mk(4097), *d = mk(4200); int r;
    CHECK(s && d); if (!s || !d) { fr(s); fr(d); return; }
    pat(s, 4096);
    d[4096] = 0x7e;
    r = _snwprintf(d, 4096, L"%ls", s);
    CHECK_EQ_I(r, 4096);
    CHECK_EQ_I(d[4096], 0x7e);
    fr(s); fr(d);
}
int main(void)
{
    tc_begin();
    TC_RUN(scwprintf_5000); TC_RUN(snwprintf_70000); TC_RUN(neg_trunc_4096); TC_RUN(exact_4096_no_nul);
    return tc_end();
}
