/* TC
 * id:    mb_neg_mbsrtowcs_invalid
 * area:  mbcs
 * funcs: setlocale
 * procs: mbsrtowcs
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.29.6.4.1 (EILSEQ); MS docs mbsrtowcs
 * note:  src-on-error and NULL src moved to mb_neg_probe_restartable_src_on_error / _null_src.
 *        Observed Win10 22H2 ucrtbase (CP932): "a\x82 b" does NOT fail (lead+0x20 is accepted, mbsrtowcs returns 3, errno 0); a lead byte
 *        followed by the terminating NUL ("a\x82") fails with (size_t)-1 and EILSEQ. The test now uses "a\x82". Stable.*/
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

TC_CASE(invalid_seq)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t d[8]; static const char s[] = "a\x82";   /* lead byte then NUL; "a\x82 b" is accepted on Windows */ const char *src = s;
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st);
    errno = 0;
    CHECK_EQ_U(tc_p_mbsrtowcs(d, &src, 8, ST(&st)), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbsrtowcs);
    TC_RUN(invalid_seq);
    return tc_end();
}
