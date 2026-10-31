/* TC
 * id:    wcs_neg_wcsncpy_s_src_unterminated
 * area:  wstring
 * funcs:
 * procs: wcsncpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s, wcsncpy_s: at most count characters are copied from src and a NUL is appended; ERANGE if dest is too small
 * note:  src is NOT NUL-terminated and ends flush against a PAGE_NOACCESS page, so any read beyond count code units crashes.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(int, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));

/* returns pointer to n wide chars filled with 'A'+i that end exactly at an inaccessible page */
static wchar_t *guard_src(size_t n)
{
    char *base = (char *)VirtualAlloc(NULL, 8192, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    DWORD old; size_t i; wchar_t *s;
    if (!base) return NULL;
    VirtualProtect(base + 4096, 4096, PAGE_NOACCESS, &old);
    s = (wchar_t *)(base + 4096 - n * sizeof(wchar_t));
    for (i = 0; i < n; i++) s[i] = (wchar_t)('A' + i);
    return s;
}

TC_CASE(unterminated_count_lt_size_ok_guard)
{
    wchar_t d[8]; wchar_t *s; int r;
    if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing");
    s = guard_src(4); if (!s) TC_SKIP("no VirtualAlloc");
    tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 8, s, 4);
    CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"ABCD"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(unterminated_count_eq_size_erange)
{
    wchar_t d[4]; wchar_t *s; int r;
    if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing");
    s = guard_src(4); if (!s) TC_SKIP("no VirtualAlloc");
    d[0] = L'x'; tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 4, s, 4);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcsncpy_s);
    TC_RUN(unterminated_count_lt_size_ok_guard); TC_RUN(unterminated_count_eq_size_erange);
   
    return tc_end();
}
