/* TC
 * id:    mb_mbsinit_state
 * area:  mbcs
 * funcs: 
 * procs: mbsinit mbrtowc
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.2.1; MS docs mbsinit
 * note:  procs because the mingw ucrtbase import lib lacks mbsinit
 */
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

typedef int (__cdecl *mbsinit_t)(const void *);
static mbsinit_t p_mbsinit;
TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));

TC_CASE(null_and_zero)
{
    mbs_t st; memset(&st, 0, sizeof st);
    if (!p_mbsinit) TC_SKIP("mbsinit not exported");
    CHECK(p_mbsinit(NULL) != 0);
    CHECK(p_mbsinit(&st) != 0);
}
TC_CASE(partial_sequence)
{
    mbs_t st; wchar_t wc = 0; char b1[1], b2[1];
    if (!p_mbsinit || !tc_p_mbrtowc) TC_SKIP("mbsinit/mbrtowc not exported");
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    b1[0] = (char)0x82; b2[0] = (char)0xA0;
    CHECK_EQ_U(tc_p_mbrtowc(&wc, b1, 1, ST(&st)), (size_t)-2);
    CHECK(p_mbsinit(&st) == 0);
    CHECK_EQ_U(tc_p_mbrtowc(&wc, b2, 1, ST(&st)), 1);
    CHECK(p_mbsinit(&st) != 0);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    p_mbsinit = (mbsinit_t)tc_crt_proc("mbsinit");
    TC_RUN(null_and_zero); TC_RUN(partial_sequence);
    return tc_end();
}
