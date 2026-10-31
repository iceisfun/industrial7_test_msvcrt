/* TC
 * id:    mb_mbsrtowcs_basic
 * area:  mbcs
 * funcs: setlocale
 * procs: mbsrtowcs
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.4.1; MS docs mbsrtowcs
 * note:*/
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

/* own state buffer: larger than any CRTs mbstate_t (ucrt: 8 bytes), avoids depending on the mingw typedef */
typedef struct { unsigned char b[16]; } mbs_t;
#define ST(p) ((mbstate_t *)(p))
static int set932(void) { return setlocale(LC_ALL, ".932") != NULL; }
static void restore_c(void) { setlocale(LC_ALL, "C"); }
TC_DECL_PROC(size_t, mbsrtowcs, (wchar_t *, const char **, size_t, void *));
#define HAVE_ALL (tc_p_mbsrtowcs)

TC_CASE(full)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t d[8]; const char *src = "abc"; memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbsrtowcs(d, &src, 8, ST(&st)), 3);
    CHECK_EQ_P(src, NULL);
    CHECK_WSTR(d, L"abc");
}
TC_CASE(limit)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t d[8]; static const char s[] = "abcd"; const char *src = s; memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbsrtowcs(d, &src, 2, ST(&st)), 2);
    CHECK_EQ_P(src, s + 2);
    CHECK_EQ_I(d[0], L'a'); CHECK_EQ_I(d[1], L'b');
}
TC_CASE(count_only)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; static const char s[] = "abcd"; const char *src = s; memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbsrtowcs(NULL, &src, 0, ST(&st)), 4);
    CHECK_EQ_P(src, s);
}
TC_CASE(cp932)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t d[8]; static const char s[] = "\x82\xA0" "a\x82\xA2"; const char *src = s;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbsrtowcs(d, &src, 8, ST(&st)), 3);
    CHECK_EQ_I(d[0], 0x3042); CHECK_EQ_I(d[1], L'a'); CHECK_EQ_I(d[2], 0x3044); CHECK_EQ_I(d[3], 0);
    CHECK_EQ_P(src, NULL);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbsrtowcs);
    TC_RUN(full); TC_RUN(limit); TC_RUN(count_only); TC_RUN(cp932);
    return tc_end();
}
