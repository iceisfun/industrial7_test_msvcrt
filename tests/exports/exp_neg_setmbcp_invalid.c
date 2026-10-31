/* TC
 * id:    exp_neg_setmbcp_invalid
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _setmbcp: returns 0 on success, -1 if the code page is invalid
 * note:  Code page 99999 is not a valid ANSI/OEM code page on any Windows.
 *        The 'current code page unchanged after failure' claim is probed in exp_neg_probe_setmbcp_unchanged.
 */
#include "tc.h"

typedef int (__cdecl *setcp_t)(int);
typedef int (__cdecl *getcp_t)(void);

TC_CASE(invalid_codepage)
{
    setcp_t s = (setcp_t)tc_crt_proc("_setmbcp"); getcp_t g = (getcp_t)tc_crt_proc("_getmbcp");
    CHECK(s && g);
    if (!(s && g)) return;
    (void)g;
    CHECK_EQ_I(s(99999), -1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(invalid_codepage); return tc_end(); }
