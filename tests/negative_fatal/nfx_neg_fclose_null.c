/* TC
 * id:    nfx_neg_fclose_null
 * area:  negative_fatal
 * kind:  negative
 * funcs: fclose
 * procs: _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fclose (NULL stream: invalid parameter handler, returns EOF, errno EINVAL)
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(fclose_null)
{
    int r; errno = 0; tc_iph_count = 0;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    r = fclose((FILE *)(volatile void *)NULL);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(fclose_null); return tc_end(); }
