/* TC
 * id:    mb_neg_probe_restartable_src_on_error
 * area:  mbcs
 * funcs: setlocale
 * procs: mbsrtowcs wcsrtombs
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.29.6.4.1, 7.29.6.4.2
 * note:  PROBES: after an encoding error the *src pointer is left at the start of the offending sequence (C11: just past the last character converted). MS behaviour on error is not documented.
 *        Observed Win10 22H2 ucrtbase (CP932): "a\x82 b" is not an error (returns 3, src becomes NULL); the error input is now "a\x82"
 *        (lead byte then NUL). Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

typedef struct { unsigned char b[16]; } mbs_t;
#define ST(p) ((mbstate_t *)(p))
static int set932(void) { return setlocale(LC_ALL, ".932") != NULL; }
static void restore_c(void) { setlocale(LC_ALL, "C"); }
TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
TC_DECL_PROC(size_t, mbrlen, (const char *, size_t, void *));
TC_DECL_PROC(size_t, mbsrtowcs, (wchar_t *, const char **, size_t, void *));
TC_DECL_PROC(size_t, wcsrtombs, (char *, const wchar_t **, size_t, void *));

TC_CASE(mbsrtowcs_src)
{
    mbs_t st; wchar_t d[8]; static const char s[] = "a\x82"; const char *src = s;   /* "a\x82 b" is accepted on Windows */
    if (!tc_p_mbsrtowcs) TC_SKIP("mbsrtowcs not exported");
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbsrtowcs(d, &src, 8, ST(&st)), (size_t)-1);
    CHECK_EQ_P(src, s + 1);
    restore_c();
}
TC_CASE(wcsrtombs_src)
{
    mbs_t st; char d[8]; static const wchar_t s[] = { L'a', 0x4E00, L'b', 0 }; const wchar_t *src = s;
    if (!tc_p_wcsrtombs) TC_SKIP("wcsrtombs not exported");
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_wcsrtombs(d, &src, 8, ST(&st)), (size_t)-1);
    CHECK_EQ_P(src, s + 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbsrtowcs);
    TC_LOAD_PROC(wcsrtombs);
    TC_RUN(mbsrtowcs_src); TC_RUN(wcsrtombs_src);
    return tc_end();
}
