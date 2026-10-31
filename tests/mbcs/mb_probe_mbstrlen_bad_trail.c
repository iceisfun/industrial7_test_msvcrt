/* TC
 * id:    mb_probe_mbstrlen_bad_trail
 * area:  mbcs
 * funcs: _mbstrlen _setmbcp
 * procs: _mbstrnlen
 * crt:   win10
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _mbstrlen: -1 and EILSEQ on invalid multibyte character
 * note:  PROBE: lead byte 0x82 followed by 0x20 (not a valid CP932 trail byte) under locale .932 makes _mbstrlen/_mbstrnlen return -1 with errno EILSEQ on ucrtbase. Older CRTs are not asserted (they may only check for a missing trail byte).
 */
#include "tc.h"
#include <errno.h>
#include <locale.h>
#include <mbstring.h>
#include <mbctype.h>

TC_DECL_PROC(size_t, _mbstrnlen, (const char *, size_t));

static int setup(void) { setlocale(LC_CTYPE, ".932"); return _setmbcp(932) == 0; }
static void teardown(void) { _setmbcp(_MB_CP_ANSI); setlocale(LC_ALL, "C"); }

TC_CASE(strlen_bad_trail)
{
    static const unsigned char s[] = { 0x82, 0x20, 0 };
    const char *volatile p = (const char *)s;
    size_t r;
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    errno = 0;
    r = _mbstrlen(p);
    CHECK_EQ_I((long long)r, -1);
    CHECK_EQ_I(errno, EILSEQ);
    teardown();
}
TC_CASE(strnlen_bad_trail)
{
    static const unsigned char s[] = { 0x82, 0x20, 0 };
    const char *volatile p = (const char *)s;
    size_t r;
    if (!tc_p__mbstrnlen) TC_SKIP("_mbstrnlen not exported");
    if (!setup()) { teardown(); TC_SKIP("cp 932 unavailable"); }
    errno = 0;
    r = tc_p__mbstrnlen(p, 10);
    CHECK_EQ_I((long long)r, -1);
    CHECK_EQ_I(errno, EILSEQ);
    teardown();
}
int main(void) { tc_begin(); TC_LOAD_PROC(_mbstrnlen); TC_RUN(strlen_bad_trail); TC_RUN(strnlen_bad_trail); return tc_end(); }
