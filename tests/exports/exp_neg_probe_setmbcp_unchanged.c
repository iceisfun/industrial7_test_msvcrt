/* TC
 * id:    exp_neg_probe_setmbcp_unchanged
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _setmbcp
 * note:  Probing: after _setmbcp(99999) fails (-1) the current MBCS code page reported by _getmbcp is unchanged.
 */
#include "tc.h"

typedef int (__cdecl *setcp_t)(int);
typedef int (__cdecl *getcp_t)(void);

TC_CASE(unchanged_after_failure)
{
    setcp_t s = (setcp_t)tc_crt_proc("_setmbcp"); getcp_t g = (getcp_t)tc_crt_proc("_getmbcp");
    int before;
    CHECK(s && g);
    if (!(s && g)) return;
    before = g();
    CHECK_EQ_I(s(99999), -1);
    CHECK_EQ_I(g(), before);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(unchanged_after_failure); return tc_end(); }
