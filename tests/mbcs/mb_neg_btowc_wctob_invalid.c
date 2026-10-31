/* TC
 * id:    mb_neg_btowc_wctob_invalid
 * area:  mbcs
 * funcs: setlocale
 * procs: btowc wctob
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.1 (not a single-byte character -> WEOF / EOF); MS docs btowc, wctob
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
TC_DECL_PROC(wint_t, btowc, (int));
TC_DECL_PROC(int, wctob, (wint_t));
#define HAVE_ALL (tc_p_btowc && tc_p_wctob)

TC_CASE(btowc_lead_byte)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    volatile int b = 0x82;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_U(tc_p_btowc(b), (wint_t)WEOF);
    restore_c();
}
TC_CASE(wctob_c_locale)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    volatile wint_t w = 0x4E00;
    CHECK_EQ_I(tc_p_wctob(w), EOF);
}
TC_CASE(wctob_needs_two_bytes)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    volatile wint_t w = 0x3042;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_I(tc_p_wctob(w), EOF);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(btowc);
    TC_LOAD_PROC(wctob);
    TC_RUN(btowc_lead_byte); TC_RUN(wctob_c_locale); TC_RUN(wctob_needs_two_bytes);
    return tc_end();
}
