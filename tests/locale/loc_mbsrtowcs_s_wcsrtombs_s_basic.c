/* TC
 * id:    loc_mbsrtowcs_s_wcsrtombs_s_basic
 * area:  locale
 * funcs: setlocale
 * procs: mbsrtowcs_s wcsrtombs_s
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs mbsrtowcs_s, wcsrtombs_s
 * note:  pReturnValue includes the terminating NUL when the whole string is converted.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

TC_DECL_PROC(errno_t, mbsrtowcs_s, (size_t *, wchar_t *, size_t, const char **, size_t, void *));
TC_DECL_PROC(errno_t, wcsrtombs_s, (size_t *, char *, size_t, const wchar_t **, size_t, void *));
static const char * volatile v_s = "abcdef";
static const wchar_t * volatile v_w = L"abcdef";

TC_CASE(mbs_full)
{
    wchar_t d[16]; const char *s = v_s; size_t n = 99; errno_t e; rst();
    e = tc_p_mbsrtowcs_s(&n, d, 16, &s, 15, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 7); CHECK(s == NULL); CHECK_WSTR(d, L"abcdef");
}
TC_CASE(wcs_full)
{
    char d[16]; const wchar_t *s = v_w; size_t n = 99; errno_t e; rst();
    e = tc_p_wcsrtombs_s(&n, d, 16, &s, 15, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 7); CHECK(s == NULL); CHECK_STR(d, "abcdef");
}
TC_CASE(count_limit_src_advanced)
{
    wchar_t d[16]; char c[16]; const char *s0 = v_s, *s = s0; const wchar_t *w0 = v_w, *w = w0; size_t n = 99; errno_t e; rst();
    e = tc_p_mbsrtowcs_s(&n, d, 16, &s, 3, PST);
    CHECK_EQ_I(e, 0); CHECK(s == s0 + 3); CHECK_EQ_U(d[0], 'a'); CHECK_EQ_U(d[2], 'c');
    rst(); n = 99;
    e = tc_p_wcsrtombs_s(&n, c, 16, &w, 3, PST);
    CHECK_EQ_I(e, 0); CHECK(w == w0 + 3); CHECK_EQ_I(c[0], 'a'); CHECK_EQ_I(c[2], 'c');
}
TC_CASE(1252_euro)
{
    static const char orig[] = "\x80";
    wchar_t d[4]; char c[4]; const char *s = orig; const wchar_t *w = d; size_t n = 99; errno_t e;
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst();
    e = tc_p_mbsrtowcs_s(&n, d, 4, &s, 3, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 2); CHECK_EQ_U(d[0], 0x20AC); CHECK_EQ_U(d[1], 0);
    rst(); n = 99;
    e = tc_p_wcsrtombs_s(&n, c, 4, &w, 3, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(n, 2); CHECK_EQ_U((unsigned char)c[0], 0x80); CHECK_EQ_I(c[1], 0);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(mbsrtowcs_s); TC_LOAD_PROC(wcsrtombs_s);
    if (!tc_p_mbsrtowcs_s || !tc_p_wcsrtombs_s) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(mbs_full); TC_RUN(wcs_full); TC_RUN(count_limit_src_advanced); TC_RUN(1252_euro);
    return tc_end();
}
