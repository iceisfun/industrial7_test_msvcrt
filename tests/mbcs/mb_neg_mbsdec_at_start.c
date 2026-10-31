/* TC
 * id:    mb_neg_mbsdec_at_start
 * area:  mbcs
 * funcs: _mbsdec _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbsdec: returns NULL if start >= current
 * note:  returns NULL, not an error indicator via errno
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static unsigned char s[] = { 'a', 'b', 'c', 0 };

TC_CASE(at_or_before_start)
{
    unsigned char *volatile p = s;
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    CHECK_EQ_P(_mbsdec(p, p), NULL);
    CHECK_EQ_P(_mbsdec(p + 1, p), NULL);
    CHECK_EQ_P(_mbsdec(p + 2, p), NULL);
    CHECK_EQ_P(_mbsdec(p, p + 1), s);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(at_or_before_start); return tc_end(); }
