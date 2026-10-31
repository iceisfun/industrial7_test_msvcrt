/* TC
 * id:    mb_wcrtomb_basic
 * area:  mbcs
 * funcs: setlocale
 * procs: wcrtomb
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.3.3; MS docs wcrtomb
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
TC_DECL_PROC(size_t, wcrtomb, (char *, wchar_t, void *));
#define HAVE_ALL (tc_p_wcrtomb)

TC_CASE(ascii)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; char b[8]; volatile wchar_t w = L'A'; memset(&st, 0, sizeof st); memset(b, 0x7f, sizeof b);
    CHECK_EQ_U(tc_p_wcrtomb(b, w, ST(&st)), 1);
    CHECK_EQ_I(b[0], 'A');
}
TC_CASE(cp932_two_bytes)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; unsigned char b[8]; volatile wchar_t w = 0x3042;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_wcrtomb((char *)b, w, ST(&st)), 2);
    CHECK_EQ_I(b[0], 0x82); CHECK_EQ_I(b[1], 0xA0);
    restore_c();
}
TC_CASE(nul)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; char b[8]; volatile wchar_t w = 0; memset(&st, 0, sizeof st); memset(b, 0x7f, sizeof b);
    CHECK_EQ_U(tc_p_wcrtomb(b, w, ST(&st)), 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buf_reset)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; volatile wchar_t w = L'x'; memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_wcrtomb(NULL, w, ST(&st)), 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcrtomb);
    TC_RUN(ascii); TC_RUN(cp932_two_bytes); TC_RUN(nul); TC_RUN(null_buf_reset);
    return tc_end();
}
