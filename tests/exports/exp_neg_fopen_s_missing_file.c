/* TC
 * id:    exp_neg_fopen_s_missing_file
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s
 * note:  returns errno value (ENOENT), *pFile = NULL, file system failures do not invoke the iph.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *fo_t)(FILE **, const char *, const char *);
static fo_t p_fo;

#define NOFILE "tc_exp_nonexistent_dir/x.tmp"
TC_CASE(enoent_returned)
{ FILE *f = (FILE *)1; CHECK_EQ_I(p_fo(&f, NOFILE, "r"), ENOENT); }
TC_CASE(stream_null)
{ FILE *f = (FILE *)1; p_fo(&f, NOFILE, "r"); CHECK(f == NULL); }
TC_CASE(no_iph)
{ FILE *f; tc_iph_count = 0; p_fo(&f, NOFILE, "r"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    p_fo = (fo_t)tc_crt_proc("fopen_s");
    if (!p_fo || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(enoent_returned); TC_RUN(stream_null); TC_RUN(no_iph);
    return tc_end();
}
