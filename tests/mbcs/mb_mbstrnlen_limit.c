/* TC
 * id:    mb_mbstrnlen_limit
 * area:  mbcs
 * funcs: _setmbcp
 * procs: _mbstrnlen
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mbstrlen, _mbstrnlen
 * note:  locale .932 selected so ucrt takes the multibyte path.
 */
#include "tc.h"
#include <locale.h>
#include <stdio.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stdlib.h>

TC_DECL_PROC(size_t, _mbstrnlen, (const char *, size_t));

static int setup(void) { setlocale(LC_CTYPE, ".932"); return _setmbcp(932) == 0; }
static void teardown(void) { _setmbcp(_MB_CP_ANSI); setlocale(LC_ALL, "C"); }

TC_CASE(ascii)
{
    const char *volatile s = "abcd";
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    CHECK_EQ_U(tc_p__mbstrnlen(s, 2), 2);
    CHECK_EQ_U(tc_p__mbstrnlen(s, 10), 4);
    CHECK_EQ_U(tc_p__mbstrnlen(s, 0), 0);
    teardown();
}
TC_CASE(dbcs)
{
    static const unsigned char s[] = { 0x82, 0xA0, 'a', 0 };
    const char *volatile p = (const char *)s;
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    CHECK_EQ_U(tc_p__mbstrnlen(p, 2), 1);
    CHECK_EQ_U(tc_p__mbstrnlen(p, 3), 2);
    CHECK_EQ_U(tc_p__mbstrnlen(p, 100), 2);
    teardown();
}
int main(void) { tc_begin(); TC_LOAD_PROC(_mbstrnlen); if (!tc_p__mbstrnlen) { tc_write("TC FAIL " TC_TEST_ID "::__import _mbstrnlen not exported\n"); return 1; } TC_RUN(ascii); TC_RUN(dbcs); return tc_end(); }
