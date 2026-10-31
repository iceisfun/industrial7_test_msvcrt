/* TC
 * id:    prc_neg_wspawnl_null_cmdname
 * area:  process
 * kind:  negative
 * funcs: _wspawnl
 * crt:   >=80
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs Parameter validation (EINVAL + invalid parameter handler); NULL filename validation is not spelled out for _wspawnl
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(null_cmdname)
{
    intptr_t r; const wchar_t *volatile nul = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wspawnl(_P_WAIT, nul, nul, (const wchar_t *)NULL);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_cmdname); return tc_end(); }
