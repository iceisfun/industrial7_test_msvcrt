/* TC
 * id:    mb_neg_wcrtomb_unmappable
 * area:  mbcs
 * funcs: wcrtomb_s
 * procs: wcrtomb
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.3.3 (EILSEQ); MS docs wcrtomb_s (EINVAL for NULL dst with size>0)
 * note:  the small-buffer iph assertion lives in mb_neg_wcrtomb_s_small_probe*/
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


TC_CASE(c_locale_unmappable)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; char b[8]; volatile wchar_t w = 0x4E00; memset(&st, 0, sizeof st);
    errno = 0;
    CHECK_EQ_U(tc_p_wcrtomb(b, w, ST(&st)), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(s_null_buf)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; size_t n = 99; volatile wchar_t w = L'A'; int r; memset(&st, 0, sizeof st);
    tc_iph_install(); tc_iph_count = 0;
    r = wcrtomb_s(&n, NULL, 8, w, ST(&st));
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcrtomb);
    TC_RUN(c_locale_unmappable); TC_RUN(s_null_buf);
    return tc_end();
}
