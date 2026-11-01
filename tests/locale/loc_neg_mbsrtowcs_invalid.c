/* TC
 * id:    loc_neg_mbsrtowcs_invalid
 * area:  locale
 * funcs: setlocale
 * procs: mbsrtowcs wcsrtombs
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.4.1 p3, 7.29.6.4.2
 * note:  Win10 triage (class A): the bad sequence is now 0xFF (observed -1/EILSEQ); lead 0x82 + 0x20 converts successfully on ucrtbase 19045
 *        (see mbs_replaced_trail). Observed on Windows 10 22H2 build 19045.
 *        The position left in *src is probed separately (loc_neg_mbsrtowcs_src_at_bad_probe).
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
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
/* 0xFF is neither a lead nor a valid single byte in cp932: Win10 ucrtbase rejects it (-1/EILSEQ). The earlier 0x82 0x20 pair is silently accepted (5 wchars, U+30FB), see mbs_replaced_trail. */
static const char bad_mbs[] = { 'a', 'b', (char)0xFF, 'c', 'd', 0 };
static const char repl_mbs[] = { 'a', 'b', (char)0x82, 0x20, 'c', 'd', 0 };
static const wchar_t bad_wcs[] = { 'a', 'b', 0x0416, 'c', 'd', 0 };

TC_CASE(mbs_invalid_trail)
{
    wchar_t d[16]; const char *s = bad_mbs; size_t r;
    CHECK(setlocale(LC_ALL, ".932") != NULL); rst();
    r = tc_p_mbsrtowcs(d, &s, 16, PST);
    CHECK_EQ_U(r, (size_t)-1);
    setlocale(LC_ALL, "C");
}
TC_CASE(mbs_replaced_trail)
{
    wchar_t d[16]; const char *s = repl_mbs; size_t r;
    CHECK(setlocale(LC_ALL, ".932") != NULL); rst();
    r = tc_p_mbsrtowcs(d, &s, 16, PST);
    CHECK_EQ_U(r, 5); CHECK_EQ_U(d[2], 0x30FB); CHECK(s == NULL);   /* observed on Win10 22H2; deviates from C11 7.29.6.4.1 */
    setlocale(LC_ALL, "C");
}
TC_CASE(wcs_unmappable)
{
    char d[16]; const wchar_t *s = bad_wcs; size_t r;
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst();
    r = tc_p_wcsrtombs(d, &s, 16, PST);
    CHECK_EQ_U(r, (size_t)-1);
    setlocale(LC_ALL, "C");
}
TC_CASE(errno_eilseq)
{
    wchar_t d[16]; char c[16]; const char *s = bad_mbs; const wchar_t *ws = bad_wcs; size_t r;
    CHECK(setlocale(LC_ALL, ".932") != NULL); rst(); errno = 0;
    r = tc_p_mbsrtowcs(d, &s, 16, PST); CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst(); errno = 0;
    r = tc_p_wcsrtombs(c, &ws, 16, PST); CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(mbsrtowcs); TC_LOAD_PROC(wcsrtombs);
    if (!tc_p_mbsrtowcs || !tc_p_wcsrtombs) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(mbs_invalid_trail); TC_RUN(mbs_replaced_trail); TC_RUN(wcs_unmappable); TC_RUN(errno_eilseq);
    return tc_end();
}
