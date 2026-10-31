/* TC
 * id:    exp_neg_fopen_s_null_pfile
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s
 * note:  NULL args -> iph + EINVAL
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *fo_t)(FILE **, const char *, const char *);
static fo_t p_fo;

TC_CASE(null_pfile_einval)
{ FILE **volatile pf = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_fo(pf, "x.tmp", "r"), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_filename_einval)
{ FILE *f = (FILE *)1; const char *volatile n = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_fo(&f, n, "r"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_mode_einval)
{ FILE *f = (FILE *)1; const char *volatile m = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_fo(&f, "x.tmp", m), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(iph_count_3)
{
    FILE *f; const char *volatile n = NULL; FILE **volatile pf = NULL;
    tc_iph_count = 0;
    p_fo(pf, "x.tmp", "r"); p_fo(&f, n, "r"); p_fo(&f, "x.tmp", n);
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin();
    p_fo = (fo_t)tc_crt_proc("fopen_s");
    if (!p_fo || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(null_pfile_einval); TC_RUN(null_filename_einval); TC_RUN(null_mode_einval); TC_RUN(iph_count_3);
    return tc_end();
}
