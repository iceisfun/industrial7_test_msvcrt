/* TC
 * id:    mb_mbscpy_cat_legacy
 * area:  mbcs
 * funcs: 
 * procs: _mbscpy _mbscat
 * crt:   win7
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbscpy, _mbscat (deprecated, removed from the UCRT)
 * note:  Resolved at run time; skipped where the DLL does not export them.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

typedef unsigned char *(__cdecl *cpy_t)(unsigned char *, const unsigned char *);
static cpy_t p_cpy, p_cat;

TC_CASE(copy)
{
    static const unsigned char src[] = { 'a', 0x82, 0xA0, 'z', 0 };
    unsigned char d[16]; int i;
    if (!p_cpy) TC_SKIP("_mbscpy not exported");
    for (i = 0; i < 16; i++) d[i] = 0xEE;
    CHECK(p_cpy(d, src) == d);
    CHECK_MEM(d, src, 5);
    CHECK_EQ_U(d[5], 0xEE);
}
TC_CASE(cat)
{
    unsigned char d[16] = { 0x82, 0xA0, 0 }; static const unsigned char s[] = { 0x82, 0xA2, 'q', 0 };
    static const unsigned char want[] = { 0x82, 0xA0, 0x82, 0xA2, 'q', 0 };
    if (!p_cat) TC_SKIP("_mbscat not exported");
    CHECK(p_cat(d, s) == d);
    CHECK_MEM(d, want, 6);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    p_cpy = (cpy_t)tc_crt_proc("_mbscpy"); p_cat = (cpy_t)tc_crt_proc("_mbscat");
    TC_RUN(copy); TC_RUN(cat);
    _setmbcp(old); return tc_end();
}
