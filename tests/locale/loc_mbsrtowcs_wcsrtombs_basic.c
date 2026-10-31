/* TC
 * id:    loc_mbsrtowcs_wcsrtombs_basic
 * area:  locale
 * funcs: setlocale
 * procs: mbsrtowcs wcsrtombs
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.4.1, 7.29.6.4.2
 * note:  
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

TC_DECL_PROC(size_t, mbsrtowcs, (wchar_t *, const char **, size_t, void *));
TC_DECL_PROC(size_t, wcsrtombs, (char *, const wchar_t **, size_t, void *));
static const char * volatile v_s = "abcdef";
static const wchar_t * volatile v_w = L"abcdef";

TC_CASE(mbs_full_src_null)
{
    wchar_t d[16]; const char *s = v_s; size_t r; rst();
    r = tc_p_mbsrtowcs(d, &s, 16, PST);
    CHECK_EQ_U(r, 6); CHECK(s == NULL); CHECK_WSTR(d, L"abcdef");
}
TC_CASE(mbs_len_limit_src_advanced)
{
    wchar_t d[16]; const char *s0 = v_s, *s = s0; size_t r; int i; rst();
    for (i = 0; i < 16; i++) d[i] = 0x7777;
    r = tc_p_mbsrtowcs(d, &s, 3, PST);
    CHECK_EQ_U(r, 3); CHECK(s == s0 + 3); CHECK_EQ_U(d[0], 'a'); CHECK_EQ_U(d[2], 'c'); CHECK_EQ_U(d[3], 0x7777);
}
TC_CASE(mbs_null_dst_length)
{
    const char *s0 = v_s, *s = s0; size_t r; rst();
    r = tc_p_mbsrtowcs(NULL, &s, 0, PST);
    CHECK_EQ_U(r, 6); CHECK(s == s0);
}
TC_CASE(wcs_full_src_null)
{
    char d[16]; const wchar_t *s = v_w; size_t r; rst();
    r = tc_p_wcsrtombs(d, &s, 16, PST);
    CHECK_EQ_U(r, 6); CHECK(s == NULL); CHECK_STR(d, "abcdef");
}
TC_CASE(wcs_len_limit)
{
    char d[16]; const wchar_t *s0 = v_w, *s = s0; size_t r; int i; rst();
    for (i = 0; i < 16; i++) d[i] = 0x55;
    r = tc_p_wcsrtombs(d, &s, 3, PST);
    CHECK_EQ_U(r, 3); CHECK(s == s0 + 3); CHECK_EQ_I(d[2], 'c'); CHECK_EQ_I(d[3], 0x55);
}
TC_CASE(cp1252_roundtrip)
{
    static const char orig[] = "caf\xE9";
    static const wchar_t wide[] = { 'c', 'a', 'f', 0x00E9, 0 };
    wchar_t w[8]; char b[8]; const char *s = orig; const wchar_t *ws = w; size_t r;
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst();
    r = tc_p_mbsrtowcs(w, &s, 8, PST);
    CHECK_EQ_U(r, 4); CHECK_WSTR(w, wide);
    rst();
    r = tc_p_wcsrtombs(b, &ws, 8, PST);
    CHECK_EQ_U(r, 4); CHECK_STR(b, orig);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(mbsrtowcs); TC_LOAD_PROC(wcsrtombs);
    if (!tc_p_mbsrtowcs || !tc_p_wcsrtombs) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(mbs_full_src_null); TC_RUN(mbs_len_limit_src_advanced); TC_RUN(mbs_null_dst_length);
    TC_RUN(wcs_full_src_null); TC_RUN(wcs_len_limit); TC_RUN(cp1252_roundtrip);
    return tc_end();
}
