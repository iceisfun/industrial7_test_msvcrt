/* TC
 * id:    mb_neg_probe_mbccpy_null_legacy
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbccpy
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: legacy _mbccpy(NULL, src) in the newer CRTs: guess that the invalid-parameter handler is invoked once and
 *        the call returns without writing. Kept in its own test because a wrong guess could fault the process.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

typedef void (__cdecl *ccpy_t)(unsigned char *, const unsigned char *);
static ccpy_t p_ccpy;
static int old_cp;
static unsigned char *volatile v_null;
static const unsigned char dbcs[] = {0x82, 0xA0, 0};

TC_CASE(legacy_null_dst_iph)
{
    if (!p_ccpy) TC_SKIP("not exported");
    tc_iph_count = 0;
    p_ccpy(v_null, dbcs);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ccpy = (ccpy_t)tc_crt_proc("_mbccpy");
    old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(legacy_null_dst_iph);
    _setmbcp(old_cp);
    return tc_end();
}
