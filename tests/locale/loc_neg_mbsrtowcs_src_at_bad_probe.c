/* TC
 * id:    loc_neg_mbsrtowcs_src_at_bad_probe
 * area:  locale
 * funcs: setlocale
 * procs: mbsrtowcs wcsrtombs
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.29.6.4.1 p3, 7.29.6.4.2
 * note:  PROBE: after an encoding error the Microsoft CRT leaves *src pointing at the offending
 *        sequence (as ISO C requires). Kept apart from the documented -1/EILSEQ checks in
 *        loc_neg_mbsrtowcs_invalid because the Windows CRT's position update is not documented.
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
static const char bad_mbs[] = { 'a', 'b', (char)0x82, 0x20, 'c', 'd', 0 };
static const wchar_t bad_wcs[] = { 'a', 'b', 0x0416, 'c', 'd', 0 };

TC_CASE(mbs_src_points_at_bad)
{
    wchar_t d[16]; const char *s = bad_mbs; size_t r;
    CHECK(setlocale(LC_ALL, ".932") != NULL); rst();
    r = tc_p_mbsrtowcs(d, &s, 16, PST); (void)r;
    CHECK(s == bad_mbs + 2);
    setlocale(LC_ALL, "C");
}
TC_CASE(wcs_src_points_at_bad)
{
    char d[16]; const wchar_t *s = bad_wcs; size_t r;
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst();
    r = tc_p_wcsrtombs(d, &s, 16, PST); (void)r;
    CHECK(s == bad_wcs + 2);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(mbsrtowcs); TC_LOAD_PROC(wcsrtombs);
    if (!tc_p_mbsrtowcs || !tc_p_wcsrtombs) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(mbs_src_points_at_bad); TC_RUN(wcs_src_points_at_bad);
    return tc_end();
}
