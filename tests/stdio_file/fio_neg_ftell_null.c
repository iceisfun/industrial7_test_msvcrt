/* TC
 * id:    fio_neg_ftell_null
 * area:  stdio_file
 * kind:  negative
 * funcs: ftell
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ftell (NULL stream -> invalid parameter handler, EINVAL, -1)
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE * volatile nf = NULL;

TC_CASE(null_stream)
{
    int before = tc_iph_count; errno = 0;
    CHECK_EQ_I(ftell(nf), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_stream); return tc_end(); }
