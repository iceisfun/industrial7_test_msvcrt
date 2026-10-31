/* TC
 * id:    mb_neg_mbstrlen_invalid_seq
 * area:  mbcs
 * funcs: _mbstrlen _setmbcp
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbstrlen, _mbstrlen_l: returns -1 and sets errno to EILSEQ if an invalid multibyte character is found
 * note:  ucrt consults the locale (MB_CUR_MAX) so the test selects locale .932 first. Restricted to win10: whether pre-ucrt CRTs return -1/EILSEQ for a trailing lead byte is not established (msvcrt.dll predates EILSEQ reporting). Lead+invalid-trail is in mb_probe_mbstrlen_bad_trail.
 */
#include "tc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stdlib.h>

static int setup(void)
{
    setlocale(LC_CTYPE, ".932");
    if (_setmbcp(932) != 0) return 0;
    return 1;
}
static void teardown(void) { _setmbcp(_MB_CP_ANSI); setlocale(LC_ALL, "C"); }

TC_CASE(lead_then_nul)
{
    static const unsigned char s[] = { 0x82, 0 };
    const char *volatile p = (const char *)s;
    size_t r;
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    errno = 0;
    r = _mbstrlen(p);
    CHECK_EQ_I((long long)r, -1);
    CHECK_EQ_I(errno, EILSEQ);
    teardown();
}
TC_CASE(valid_still_works)
{
    static const unsigned char s[] = { 0x82, 0xA0, 'a', 0 };
    const char *volatile p = (const char *)s;
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    CHECK_EQ_U(_mbstrlen(p), 2);
    teardown();
}
int main(void) { tc_begin(); TC_RUN(lead_then_nul); TC_RUN(valid_still_works); return tc_end(); }
