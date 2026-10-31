/* TC
 * id:    mb_neg_wcrtomb_s_small_probe
 * area:  mbcs
 * funcs: wcrtomb_s setlocale
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcrtomb_s: insufficient buffer is a parameter error
 * note:  PROBES: wcrtomb_s with a 1-byte buffer for a 2-byte cp932 char returns ERANGE or EINVAL (either accepted), and whether the invalid-parameter handler is called (expected: yes, count>=1).
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

TC_CASE(small_buffer)
{
    mbs_t st; char b[1]; size_t n = 99; volatile wchar_t w = 0x3042; int r;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    tc_iph_install(); tc_iph_count = 0;
    r = wcrtomb_s(&n, b, 1, w, ST(&st));
    CHECK(r == ERANGE || r == EINVAL);
    CHECK(tc_iph_count >= 1);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_RUN(small_buffer);
    return tc_end();
}
