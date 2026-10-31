/* TC
 * id:    mb_btowc_wctob
 * area:  mbcs
 * funcs: 
 * procs: btowc wctob
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.1.1, 7.29.6.1.2; MS docs btowc, wctob
 * note:  msvcr100 lacks these per the import lib*/
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
TC_DECL_PROC(wint_t, btowc, (int));
TC_DECL_PROC(int, wctob, (wint_t));
#define HAVE_ALL (tc_p_btowc && tc_p_wctob)

TC_CASE(btowc_basic)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    volatile int a = 'A', z = 0, e = EOF;
    CHECK_EQ_I(tc_p_btowc(a), L'A');
    CHECK_EQ_I(tc_p_btowc(z), 0);
    CHECK_EQ_U(tc_p_btowc(e), (wint_t)WEOF);
}
TC_CASE(wctob_basic)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    volatile wint_t a = L'A', w = WEOF;
    CHECK_EQ_I(tc_p_wctob(a), 'A');
    CHECK_EQ_I(tc_p_wctob(w), EOF);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(btowc);
    TC_LOAD_PROC(wctob);
    TC_RUN(btowc_basic); TC_RUN(wctob_basic);
    return tc_end();
}
