/* TC
 * id:    wcs_neg_nul_stop_no_overread
 * area:  wstring
 * funcs: wcsncmp _wcsnicmp wcsncpy wcsncat
 * procs: wcsnlen
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.4.3 (wcsncmp: characters that follow a null wide character are not compared), 7.29.4.2.2, 7.29.4.3.2; MS docs wcsnlen
 * note:  Strings are placed so that their NUL is the last wchar of a committed page followed by a PAGE_NOACCESS page.
 *        A huge count (SIZE_MAX) must still stop at the NUL; an access violation crashes the test (= failure).
 *        wcsncpy uses count == len+1 (SIZE_MAX would zero-fill the destination).
 */
#include "tc.h"
#include <wchar.h>

#define PAGE 4096
#define WPP (PAGE / sizeof(wchar_t))
static wchar_t *page0, *page2;
static volatile size_t huge = (size_t)-1;

static wchar_t *place(wchar_t *page, int len, wchar_t base)
{
    wchar_t *s = page + WPP - 1 - len; int i;
    for (i = 0; i < len; i++) s[i] = (wchar_t)(base + (i % 20));
    s[len] = 0;
    return s;
}
TC_CASE(wcsncmp_equal_huge_n)
{
    int len, bad = 0;
    for (len = 0; len < 40; len++) {
        wchar_t *volatile a = place(page0, len, L'a'), *volatile b = place(page2, len, L'a');
        if (wcsncmp(a, b, huge) != 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsnicmp_equal_huge_n)
{
    int len, bad = 0;
    for (len = 0; len < 40; len++) {
        wchar_t *volatile a = place(page0, len, L'a'), *volatile b = place(page2, len, L'A');
        if (_wcsnicmp(a, b, huge) != 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsncat_src_nul_at_guard)
{
    int len, bad = 0;
    for (len = 0; len < 40; len++) {
        wchar_t d[64]; wchar_t *volatile s = place(page0, len, L'a'); int i;
        d[0] = L'Q'; d[1] = 0;
        wcsncat(d, s, huge);
        if (d[0] != L'Q' || d[len + 1] != 0) bad++;
        for (i = 0; i < len; i++) if (d[i + 1] != s[i]) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsncpy_src_nul_last_count_len_plus1)
{
    int len, bad = 0;
    for (len = 0; len < 40; len++) {
        wchar_t d[64]; wchar_t *volatile s = place(page0, len, L'a'); int i;
        for (i = 0; i < 64; i++) d[i] = 0x5555;
        wcsncpy(d, s, (size_t)len + 1);
        if (d[len] != 0 || d[len + 1] != 0x5555) bad++;
        for (i = 0; i < len; i++) if (d[i] != s[i]) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsncmp_embedded_nul_differs_after)
{
    wchar_t a[6] = { L'a', L'b', 0, L'X', L'1', 0 }, b[6] = { L'a', L'b', 0, L'Y', L'2', 0 };
    wchar_t *volatile pa = a, *volatile pb = b;
    CHECK_EQ_I(wcsncmp(pa, pb, huge), 0);
    CHECK_EQ_I(_wcsnicmp(pa, pb, huge), 0);
    CHECK_EQ_I(wcsncmp(pa, pb, 6), 0);
}
TC_CASE(wcsnlen_huge_n_proc)
{
    size_t (__cdecl *p)(const wchar_t *, size_t) = (size_t (__cdecl *)(const wchar_t *, size_t))tc_crt_proc("wcsnlen");
    int len, bad = 0;
    if (!p) TC_SKIP("wcsnlen not exported");
    for (len = 0; len < 40; len++) {
        wchar_t *volatile s = place(page0, len, L'a');
        if (p(s, huge) != (size_t)len) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
int main(void)
{
    char *base; DWORD old;
    tc_begin();
    base = (char *)VirtualAlloc(NULL, 4 * PAGE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!base) { tc_write("TC FAIL " TC_TEST_ID "::VirtualAlloc\n"); return 1; }
    VirtualProtect(base + PAGE, PAGE, PAGE_NOACCESS, &old);
    VirtualProtect(base + 3 * PAGE, PAGE, PAGE_NOACCESS, &old);
    page0 = (wchar_t *)base; page2 = (wchar_t *)(base + 2 * PAGE);
    TC_RUN(wcsncmp_equal_huge_n); TC_RUN(wcsnicmp_equal_huge_n); TC_RUN(wcsncat_src_nul_at_guard);
    TC_RUN(wcsncpy_src_nul_last_count_len_plus1); TC_RUN(wcsncmp_embedded_nul_differs_after); TC_RUN(wcsnlen_huge_n_proc);
    return tc_end();
}
