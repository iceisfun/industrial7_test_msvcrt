/* TC
 * id:    mb_neg_mbstrnlen_invalid
 * area:  mbcs
 * procs: _mbstrnlen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbstrlen, _mbstrnlen: NULL is an invalid parameter (return -1)
 * note:  _mbstrnlen via procs because mingw libmsvcr80.a lacks it. NULL errno value is not asserted. Invalid-trail case moved to mb_probe_mbstrlen_bad_trail.
 */
#include "tc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stdlib.h>

TC_DECL_PROC(size_t, _mbstrnlen, (const char *, size_t));

static int setup(void) { setlocale(LC_CTYPE, ".932"); return _setmbcp(932) == 0; }
static void teardown(void) { _setmbcp(_MB_CP_ANSI); setlocale(LC_ALL, "C"); }

TC_CASE(null_arg)
{
    const char *volatile p = NULL;
    size_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = tc_p__mbstrnlen(p, 5);
    CHECK_EQ_I((long long)r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_mbstrnlen); if (!tc_p__mbstrnlen) { tc_write("TC FAIL " TC_TEST_ID "::__import _mbstrnlen not exported\n"); return 1; } TC_RUN(null_arg); return tc_end(); }
