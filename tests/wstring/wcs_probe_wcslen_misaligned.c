/* TC
 * id:    wcs_probe_wcslen_misaligned
 * area:  wstring
 * funcs: wcslen wcscmp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   (none; x86/x64 tolerate unaligned loads)
 * note:  PROBE: wcslen/wcscmp on wide strings starting at an odd byte address. Expected to work on x86/x64
 *        (the CRT SIMD paths use unaligned loads or byte-safe preambles). Page-boundary behaviour is in wcs_wcslen_page_boundary.
 */
#include "tc.h"
#include <wchar.h>

static void put(unsigned char *p, const wchar_t *s)
{
    for (;; s++) { p[0] = (unsigned char)(*s & 0xFF); p[1] = (unsigned char)((*s >> 8) & 0xFF); p += 2; if (!*s) break; }
}
static unsigned char raw1[256], raw2[256];

TC_CASE(wcslen_odd)
{
    const wchar_t *volatile p;
    put(raw1 + 1, L"hello, misaligned world"); p = (const wchar_t *)(raw1 + 1);
    CHECK_EQ_U(wcslen(p), 23);
    put(raw1 + 1, L""); CHECK_EQ_U(wcslen(p), 0);
    put(raw1 + 3, L"\x0102\x0304x"); p = (const wchar_t *)(raw1 + 3); CHECK_EQ_U(wcslen(p), 3);
}
TC_CASE(wcscmp_both_odd)
{
    const wchar_t *volatile a, *volatile b;
    put(raw1 + 1, L"alpha beta gamma"); put(raw2 + 1, L"alpha beta gamma"); a = (const wchar_t *)(raw1 + 1); b = (const wchar_t *)(raw2 + 1);
    CHECK_EQ_I(wcscmp(a, b), 0);
    put(raw2 + 1, L"alpha beta gammb"); CHECK(wcscmp(a, b) < 0); CHECK(wcscmp(b, a) > 0);
}
TC_CASE(wcscmp_mixed_alignment)
{
    const wchar_t *volatile a, *volatile b;
    put(raw1 + 1, L"0123456789abcdef!"); put(raw2, L"0123456789abcdef!"); a = (const wchar_t *)(raw1 + 1); b = (const wchar_t *)raw2;
    CHECK_EQ_I(wcscmp(a, b), 0);
    put(raw2, L"0123456789abcdeg!"); CHECK(wcscmp(a, b) < 0);
}
int main(void) { tc_begin(); TC_RUN(wcslen_odd); TC_RUN(wcscmp_both_odd); TC_RUN(wcscmp_mixed_alignment); return tc_end(); }
