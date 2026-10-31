/* TC
 * id:    fio_neg_fopen_s_null_pfile
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s: error conditions (pFile NULL -> EINVAL)
 * note:  fopen_s is resolved at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static const char * volatile fname = "tc_fio_neg_fopen_s_null_pfile_a.tmp";

TC_CASE(null_pfile)
{
    int r, n0 = tc_iph_count;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(NULL, fname, "r");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(null_pfile_write_no_file)
{
    int r, n0 = tc_iph_count;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(NULL, fname, "w");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_EQ_U(GetFileAttributesA(fname), INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    TC_LOAD_PROC(fopen_s);
    CHECK(tc_p_fopen_s != NULL);
    CHECK(tc_iph_install());
    TC_RUN(null_pfile); TC_RUN(null_pfile_write_no_file);
    DeleteFileA(fname);
    return tc_end();
}
