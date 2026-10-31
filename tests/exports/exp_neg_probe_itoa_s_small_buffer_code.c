/* TC
 * id:    exp_neg_probe_itoa_s_small_buffer_code
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _itoa_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _itoa_s (table says EINVAL for small buffer)
 * note:  PROBES: _itoa_s with a too-small buffer returns ERANGE with errno ERANGE (MS docs table says EINVAL; believed ERANGE in the real code).
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *itoa_t)(int, char *, size_t, int);
static itoa_t p_itoa;

TC_CASE(size3_erange)
{ char b[3]; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_itoa(12345, b, sizeof b, 10), ERANGE); CHECK_EQ_I(errno, ERANGE); }
int main(void)
{
    tc_begin();
    p_itoa = (itoa_t)tc_crt_proc("_itoa_s");
    if (!p_itoa || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(size3_erange);
    return tc_end();
}
