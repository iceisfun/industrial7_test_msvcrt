/* TC
 * id:    exp_neg_probe_fopen_s_null_args_pfile
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fopen_s
 * note:  PROBES: *pFile is set to NULL when filename or mode is NULL . NOTE: MS docs table says "unchanged" for these cases; the real code is believed to store NULL first. Real Windows decides.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *fo_t)(FILE **, const char *, const char *);
static fo_t p_fo;

TC_CASE(null_filename_pfile_null)
{ FILE *f = (FILE *)1; const char *volatile n = NULL; p_fo(&f, n, "r"); CHECK(f == NULL); }
TC_CASE(null_mode_pfile_null)
{ FILE *f = (FILE *)1; const char *volatile m = NULL; p_fo(&f, "x.tmp", m); CHECK(f == NULL); }
int main(void)
{
    tc_begin();
    p_fo = (fo_t)tc_crt_proc("fopen_s");
    if (!p_fo || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(null_filename_pfile_null); TC_RUN(null_mode_pfile_null);
    return tc_end();
}
