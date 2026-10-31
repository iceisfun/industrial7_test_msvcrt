/* TC
 * id:    wcs_neg_count_bound_no_overread
 * area:  wstring
 * funcs: wcsncmp _wcsnicmp wcsncpy wcsncat
 * procs: wcsnlen _wcsncoll _wcsnicoll
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.4.3, 7.29.4.2.2, 7.29.4.3.2 (at most n wide characters examined/copied); MS docs wcsnlen, _wcsncoll, _wcsnicoll
 * note:  Unterminated wide buffers end exactly at a PAGE_NOACCESS guard page and count == buffer length; the CRT must not
 *        touch the guard page. An access violation crashes the test (= failure).
 */
#include "tc.h"
#include <wchar.h>

#define PAGE 4096
#define WPP (PAGE / sizeof(wchar_t))
static wchar_t *page0, *page2;

/* len wchars, no NUL, the last one is the last wchar of the page */
static wchar_t *place(wchar_t *page, int len, wchar_t base)
{
    wchar_t *s = page + WPP - len; int i;
    for (i = 0; i < len; i++) s[i] = (wchar_t)(base + (i % 20));
    return s;
}
TC_CASE(wcsncmp_unterminated_n_eq_len)
{
    int len, bad = 0;
    for (len = 1; len < 40; len++) {
        wchar_t *volatile a = place(page0, len, L'a'), *volatile b = place(page2, len, L'a');
        if (wcsncmp(a, b, (size_t)len) != 0) bad++;
        b[len - 1] = L'~';
        if (wcsncmp(a, b, (size_t)len) >= 0) bad++;
        if (wcsncmp(b, a, (size_t)len) <= 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsnicmp_unterminated)
{
    int len, bad = 0;
    for (len = 1; len < 40; len++) {
        wchar_t *volatile a = place(page0, len, L'a'), *volatile b = place(page2, len, L'A');
        if (_wcsnicmp(a, b, (size_t)len) != 0) bad++;
        b[len - 1] = L'~';
        if (_wcsnicmp(a, b, (size_t)len) >= 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsncpy_unterminated_src)
{
    int len, bad = 0;
    for (len = 1; len < 40; len++) {
        wchar_t d[64]; wchar_t *volatile s = place(page0, len, L'a'); int i;
        for (i = 0; i < 64; i++) d[i] = 0x5555;
        wcsncpy(d, s, (size_t)len);
        if (d[len] != 0x5555) bad++;   /* no NUL added, nothing beyond count written */
        for (i = 0; i < len; i++) if (d[i] != s[i]) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsncat_unterminated_src)
{
    int len, bad = 0;
    for (len = 1; len < 40; len++) {
        wchar_t d[64]; wchar_t *volatile s = place(page0, len, L'a'); int i;
        for (i = 0; i < 64; i++) d[i] = 0x5555;
        d[0] = L'Q'; d[1] = 0;
        wcsncat(d, s, (size_t)len);
        if (d[0] != L'Q' || d[len + 1] != 0) bad++;   /* result is always NUL-terminated */
        for (i = 0; i < len; i++) if (d[i + 1] != s[i]) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsnlen_unterminated_proc)
{
    size_t (__cdecl *p)(const wchar_t *, size_t) = (size_t (__cdecl *)(const wchar_t *, size_t))tc_crt_proc("wcsnlen");
    int len, bad = 0;
    if (!p) TC_SKIP("wcsnlen not exported");
    for (len = 1; len < 40; len++) {
        wchar_t *volatile s = place(page0, len, L'a');
        if (p(s, (size_t)len) != (size_t)len) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsncoll_unterminated)
{
    int (__cdecl *c)(const wchar_t *, const wchar_t *, size_t) = (int (__cdecl *)(const wchar_t *, const wchar_t *, size_t))tc_crt_proc("_wcsncoll");
    int (__cdecl *ci)(const wchar_t *, const wchar_t *, size_t) = (int (__cdecl *)(const wchar_t *, const wchar_t *, size_t))tc_crt_proc("_wcsnicoll");
    int len, bad = 0;
    if (!c || !ci) TC_SKIP("_wcsncoll/_wcsnicoll not exported");
    for (len = 1; len < 40; len++) {
        wchar_t *volatile a = place(page0, len, L'a'), *volatile b = place(page2, len, L'A');
        if (c(a, a, (size_t)len) != 0) bad++;
        if (ci(a, b, (size_t)len) != 0) bad++;
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
    TC_RUN(wcsncmp_unterminated_n_eq_len); TC_RUN(wcsnicmp_unterminated); TC_RUN(wcsncpy_unterminated_src);
    TC_RUN(wcsncat_unterminated_src); TC_RUN(wcsnlen_unterminated_proc); TC_RUN(wcsncoll_unterminated);
    return tc_end();
}
