/* TC
 * id:    mb_mbrlen_mbrtowc_basic
 * area:  mbcs
 * funcs: setlocale
 * procs: mbrtowc mbrlen
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.3.2, 7.29.6.3.1; MS docs mbrtowc, mbrlen
 * note:  msvcr100 and older lack mbrtowc (mingw stub only)*/
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
TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
TC_DECL_PROC(size_t, mbrlen, (const char *, size_t, void *));
#define HAVE_ALL (tc_p_mbrtowc && tc_p_mbrlen)

TC_CASE(ascii)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t wc = 0; volatile char a = 'A'; char s[2]; s[0] = a; s[1] = 0;
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbrtowc(&wc, s, 1, ST(&st)), 1);
    CHECK_EQ_I(wc, L'A');
}
TC_CASE(nul)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t wc = 0x55; volatile char z = 0; char s[2]; s[0] = z; s[1] = 0;
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbrtowc(&wc, s, 1, ST(&st)), 0);
    CHECK_EQ_I(wc, 0);
}
TC_CASE(null_dst)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; volatile char a = 'a'; char s[2]; s[0] = a; s[1] = 0;
    memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbrtowc(NULL, s, 1, ST(&st)), 1);
}
TC_CASE(cp932_split)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t wc = 0; char b1[1], b2[1]; size_t r;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    b1[0] = (char)0x82; b2[0] = (char)0xA0;
    r = tc_p_mbrtowc(&wc, b1, 1, ST(&st));
    CHECK_EQ_U(r, (size_t)-2);
    r = tc_p_mbrtowc(&wc, b2, 1, ST(&st));
    CHECK_EQ_U(r, 1);
    CHECK_EQ_I(wc, 0x3042);
    restore_c();
}
TC_CASE(cp932_mbrlen_pair)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; char s[3]; if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    s[0] = (char)0x82; s[1] = (char)0xA0; s[2] = 0;
    CHECK_EQ_U(tc_p_mbrlen(s, 2, ST(&st)), 2);
    memset(&st, 0, sizeof st);
    s[0] = 'x';
    CHECK_EQ_U(tc_p_mbrlen(s, 1, ST(&st)), 1);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_LOAD_PROC(mbrlen);
    TC_RUN(ascii); TC_RUN(nul); TC_RUN(null_dst); TC_RUN(cp932_split); TC_RUN(cp932_mbrlen_pair);
    return tc_end();
}
