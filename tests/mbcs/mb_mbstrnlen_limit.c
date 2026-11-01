/* TC
 * id:    mb_mbstrnlen_limit
 * area:  mbcs
 * funcs: _setmbcp
 * procs: _mbstrnlen
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _mbstrlen, _mbstrnlen
 * note:  locale .932 selected so ucrt takes the multibyte path.
 *        Observed Win10 22H2 ucrtbase (locale .932 + _setmbcp(932), zero-padded 16-byte buffers so no read goes past the object):
 *          abcd: n=2 -> 2, n=8 -> 4, n=0 -> (size_t)-1 with errno EILSEQ (not 0).
 *          {82 A0 'a' 0}: n=1 -> -1 (split lead byte, EILSEQ), n=2 -> 2, n=3 -> 3 (when the NUL is not reached inside n the result is the
 *          BYTE count n, not the character count 1 / 2 the MS docs imply), n=4 -> 2, n=8 -> 2 (NUL reached: character count).
 *        Deviates from the MS docs for _mbstrnlen (number of multibyte characters). Stable (static tables). The original test also
 *        read past the end of its 4-byte object with n=100; the padded buffers remove that.
 */
#include "tc.h"
#include <locale.h>
#include <stdio.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(size_t, _mbstrnlen, (const char *, size_t));

static int setup(void) { setlocale(LC_CTYPE, ".932"); return _setmbcp(932) == 0; }
static void teardown(void) { _setmbcp(_MB_CP_ANSI); setlocale(LC_ALL, "C"); }

TC_CASE(ascii)
{
    static const char init[16] = "abcd";   /* zero padded */
    char buf[16]; int i; const char *volatile s = buf;
    for (i = 0; i < 16; i++) buf[i] = init[i];
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    CHECK_EQ_U(tc_p__mbstrnlen(s, 2), 2);
    CHECK_EQ_U(tc_p__mbstrnlen(s, 8), 4);
    errno = 0;
    CHECK_EQ_U(tc_p__mbstrnlen(s, 0), (size_t)-1);   /* observed: not 0 */
    CHECK_EQ_I(errno, EILSEQ);
    teardown();
}
TC_CASE(dbcs)
{
    unsigned char buf[16]; int i; const char *volatile p = (const char *)buf;
    for (i = 0; i < 16; i++) buf[i] = 0;
    buf[0] = 0x82; buf[1] = 0xA0; buf[2] = 'a';
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    errno = 0;
    CHECK_EQ_U(tc_p__mbstrnlen(p, 1), (size_t)-1);   /* split lead byte */
    CHECK_EQ_I(errno, EILSEQ);
    CHECK_EQ_U(tc_p__mbstrnlen(p, 2), 2);   /* observed: byte count, docs imply 1 */
    CHECK_EQ_U(tc_p__mbstrnlen(p, 3), 3);   /* observed: byte count, docs imply 2 */
    CHECK_EQ_U(tc_p__mbstrnlen(p, 4), 2);
    CHECK_EQ_U(tc_p__mbstrnlen(p, 8), 2);
    teardown();
}
int main(void) { tc_begin(); TC_LOAD_PROC(_mbstrnlen); if (!tc_p__mbstrnlen) { tc_write("TC FAIL " TC_TEST_ID "::__import _mbstrnlen not exported\n"); return 1; } TC_RUN(ascii); TC_RUN(dbcs); return tc_end(); }
