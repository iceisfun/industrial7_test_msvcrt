/* TC
 * id:    wcs_wcslen_page_boundary
 * area:  wstring
 * funcs: wcslen wcschr wcsrchr wcscmp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   (none; a CRT must not read beyond the terminating NUL across a page boundary)
 * note:  PROBE: strings whose NUL is the last element of a committed page, followed by a PAGE_NOACCESS page. Vectorised
 *        implementations are expected to use aligned blocks that never cross a page, so no access violation occurs.
 *        An AV here crashes the test (reported as failure).
 */
#include "tc.h"
#include <wchar.h>

static wchar_t *pg_end;  /* two-string arena */
static wchar_t *page0, *page2;
#define PAGE 4096
#define WPP (PAGE / sizeof(wchar_t))

/* place string s of len characters so that its NUL is the last wchar of the page */
static wchar_t *place(wchar_t *page, int len, wchar_t base)
{
    wchar_t *s = page + WPP - 1 - len; int i;
    for (i = 0; i < len; i++) s[i] = (wchar_t)(base + (i % 20));
    s[len] = 0;
    return s;
}
TC_CASE(wcslen_guard)
{
    int len, bad = 0;
    for (len = 0; len < 70; len++) { wchar_t *volatile s = place(page0, len, L'a'); if (wcslen(s) != (size_t)len) bad++; }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcschr_guard)
{
    int len, bad = 0;
    for (len = 1; len < 70; len++) {
        wchar_t *volatile s = place(page0, len, L'a');
        if (wcschr(s, L'a') != s) bad++;
        if (wcschr(s, (wchar_t)(L'a' + ((len - 1) % 20))) == NULL) bad++;
        if (wcschr(s, L'Z') != NULL) bad++;
        if (wcschr(s, 0) != s + len) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcsrchr_guard)
{
    int len, bad = 0;
    for (len = 1; len < 70; len++) {
        wchar_t *volatile s = place(page0, len, L'a');
        if (wcsrchr(s, L'Z') != NULL) bad++;
        if (wcsrchr(s, 0) != s + len) bad++;
        if (wcsrchr(s, (wchar_t)(L'a' + ((len - 1) % 20))) != s + len - 1) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(wcscmp_guard)
{
    int len, bad = 0;
    for (len = 0; len < 70; len++) {
        wchar_t *volatile a = place(page0, len, L'a'), *volatile b = place(page2, len, L'a');
        if (wcscmp(a, b) != 0) bad++;
        if (len > 0) { b[len - 1] = L'~'; if (wcscmp(a, b) >= 0) bad++; if (wcscmp(b, a) <= 0) bad++; }
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
    page0 = (wchar_t *)base; page2 = (wchar_t *)(base + 2 * PAGE); pg_end = page0;
    TC_RUN(wcslen_guard); TC_RUN(wcschr_guard); TC_RUN(wcsrchr_guard); TC_RUN(wcscmp_guard);
    return tc_end();
}
