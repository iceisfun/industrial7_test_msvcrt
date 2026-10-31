/* TC
 * id:    mb_neg_mbrtowc_invalid
 * area:  mbcs
 * funcs: setlocale
 * procs: mbrtowc mbrlen
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.3.2 (EILSEQ, -1, -2); MS docs mbrtowc
 * note:  n==0 (-2) moved to mb_probe_restartable_n_zero. after an error the conversion state is unspecified so each call uses a fresh state*/
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

TC_CASE(invalid_trail)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t wc = 0; char s[3];
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    s[0] = (char)0x82; s[1] = 0x20; s[2] = 0;   /* 0x20 is not a valid trail byte */
    errno = 0;
    CHECK_EQ_U(tc_p_mbrtowc(&wc, s, 2, ST(&st)), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    restore_c();
}
TC_CASE(mbrlen_invalid)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; char s[3];
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    s[0] = (char)0x82; s[1] = 0x20; s[2] = 0;
    errno = 0;
    CHECK_EQ_U(tc_p_mbrlen(s, 2, ST(&st)), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_LOAD_PROC(mbrlen);
    TC_RUN(invalid_trail); TC_RUN(mbrlen_invalid);
    return tc_end();
}
