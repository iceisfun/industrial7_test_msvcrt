/* TC
 * id:    mb_probe_mbsncpy_s_count_zero
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbsncpy_s _mbsnbcpy_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   none for the mbs forms (strncpy_s documents count==0 as "dst[0] = 0, return 0")
 * note:  PROBE: with count == 0 and valid dst/size, _mbsnbcpy_s and _mbsncpy_s return 0 and set dst[0] = 0 (guess, by
 *        analogy with strncpy_s; the mbs forms might instead leave dst untouched).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

typedef int (__cdecl *cpy_s_t)(unsigned char *, size_t, const unsigned char *, size_t);
static cpy_s_t p_ncpy_s, p_nbcpy_s;
static int old_cp;

TC_CASE(nbcpy_s_count_zero_probe)
{
    unsigned char dst[4] = {'x', 'y', 'z', 0}; const unsigned char src[] = "abc";
    volatile size_t n = 0;
    if (!p_nbcpy_s) TC_SKIP("not exported");
    CHECK_EQ_I(p_nbcpy_s(dst, sizeof dst, src, n), 0); CHECK_EQ_U(dst[0], 0);
}
TC_CASE(ncpy_s_count_zero_probe)
{
    unsigned char dst[4] = {'x', 'y', 'z', 0}; const unsigned char src[] = "abc";
    volatile size_t n = 0;
    if (!p_ncpy_s) TC_SKIP("not exported");
    CHECK_EQ_I(p_ncpy_s(dst, sizeof dst, src, n), 0); CHECK_EQ_U(dst[0], 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ncpy_s = (cpy_s_t)tc_crt_proc("_mbsncpy_s"); p_nbcpy_s = (cpy_s_t)tc_crt_proc("_mbsnbcpy_s");
    old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(nbcpy_s_count_zero_probe); TC_RUN(ncpy_s_count_zero_probe);
    _setmbcp(old_cp);
    return tc_end();
}
