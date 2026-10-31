/* TC
 * id:    fio_neg_fopen_s_null_args
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fopen_s fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s: *pFile is NULL on error
 * note:  Sentinel non-NULL value is stored in the FILE* first so a reset to NULL is observable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
TC_DECL_PROC(int, fclose, (FILE *));
static const char * volatile fname = "tc_fio_neg_fopen_s_null_args_a.tmp";
static const char * volatile nul = NULL;

TC_CASE(null_filename)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1000; int r, n0 = tc_iph_count;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, nul, "r");
    CHECK_EQ_I(r, EINVAL);
    CHECK(f == NULL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(null_mode)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1000; int r, n0 = tc_iph_count;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, fname, nul);
    CHECK_EQ_I(r, EINVAL);
    CHECK(f == NULL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_EQ_U(GetFileAttributesA(fname), INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    TC_LOAD_PROC(fopen_s); TC_LOAD_PROC(fclose);
    CHECK(tc_p_fopen_s != NULL);
    CHECK(tc_iph_install());
    TC_RUN(null_filename); TC_RUN(null_mode);
    DeleteFileA(fname);
    return tc_end();
}
