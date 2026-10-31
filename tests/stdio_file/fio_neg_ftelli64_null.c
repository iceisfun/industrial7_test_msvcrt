/* TC
 * id:    fio_neg_ftelli64_null
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: _ftelli64
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ftelli64 (NULL stream -> invalid parameter handler, EINVAL, -1)
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(long long, _ftelli64, (FILE *));
static FILE * volatile nf = NULL;

TC_CASE(null_stream)
{
    int before = tc_iph_count;
    if (!tc_p__ftelli64) TC_SKIP("not exported");
    errno = 0;
    CHECK_EQ_I(tc_p__ftelli64(nf), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_ftelli64); TC_RUN(null_stream); return tc_end(); }
