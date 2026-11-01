/* TC
 * id:    mb_neg_mbrtowc_invalid
 * area:  mbcs
 * funcs: setlocale
 * procs: mbrtowc mbrlen
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.29.6.3.2 (EILSEQ, -1, -2); MS docs mbrtowc
 * note:  n==0 (-2) moved to mb_probe_restartable_n_zero. after an error the conversion state is unspecified so each call uses a fresh state
 *        Observed Win10 22H2 ucrtbase (CP932): a lead byte followed by a NON-MAPPING trail such as 0x82 0x20 is NOT rejected: mbrtowc/mbrlen
 *        return 2 and leave errno untouched (the pair is converted to the default char); deviates from C11 7.29.6.3.2. A lead byte followed by
 *        0x00 IS rejected: (size_t)-1 with errno == EILSEQ. The invalid-sequence cases therefore use 0x82 0x00; case trail_20_accepted pins the
 *        deviation. Stable (static table).*/
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
    s[0] = (char)0x82; s[1] = 0x00; s[2] = 0;   /* lead byte + NUL: invalid on Windows (0x20 is accepted, see trail_20_accepted) */
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
    s[0] = (char)0x82; s[1] = 0x00; s[2] = 0;
    errno = 0;
    CHECK_EQ_U(tc_p_mbrlen(s, 2, ST(&st)), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    restore_c();
}
TC_CASE(trail_20_accepted)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; wchar_t wc = 0; char s[3];
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    s[0] = (char)0x82; s[1] = 0x20; s[2] = 0;   /* observed: accepted, deviates from C11 */
    memset(&st, 0, sizeof st); errno = 0;
    CHECK_EQ_U(tc_p_mbrtowc(&wc, s, 2, ST(&st)), 2);
    CHECK_EQ_I(errno, 0);
    memset(&st, 0, sizeof st); errno = 0;
    CHECK_EQ_U(tc_p_mbrlen(s, 2, ST(&st)), 2);
    CHECK_EQ_I(errno, 0);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_LOAD_PROC(mbrlen);
    TC_RUN(invalid_trail); TC_RUN(mbrlen_invalid); TC_RUN(trail_20_accepted);
    return tc_end();
}
