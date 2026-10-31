/* TC
 * id:    mb_neg_mbstrlen_invalid_932
 * area:  mbcs
 * funcs: _mbstrlen setlocale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbstrlen: returns -1 and sets errno to EILSEQ on an invalid multibyte character
 * note:  PROBE: a dangling lead byte {0x82,0} under the cp932 locale is treated as invalid by _mbstrlen (returns (size_t)-1, errno EILSEQ). Real Windows decides.
 */
#include "tc.h"
#include <mbstring.h>
#include <locale.h>
#include <stdio.h>
#include <errno.h>

TC_CASE(dangling_lead) { static const unsigned char s[] = { 0x82, 0, 0 };
    size_t r;
    if (!setlocale(LC_CTYPE, "Japanese_Japan.932")) TC_SKIP("cp932 locale unavailable");
    errno = 0;
    r = _mbstrlen((const char *)s);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(r, (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
}

int main(void) { tc_begin(); TC_RUN(dangling_lead); return tc_end(); }
