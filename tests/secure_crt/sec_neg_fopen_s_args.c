/* TC
 * id:    sec_neg_fopen_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: fopen_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s: EINVAL (and invalid-parameter handler) for NULL pFile / filename / mode; contents of *pFile are not asserted for parameter errors (see sec_neg_fopen_s_null_pfile_probe); missing file for "r" gives ENOENT
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *fopen_s_t)(void **, const char *, const char *);
typedef int *(__cdecl *errno_fn)(void);
static fopen_s_t p_fopen_s; static errno_fn p_errno;

TC_CASE(null_pfile)
{ int r; tc_iph_count = 0; r = p_fopen_s(NULL, "tc_sec_neg_fopen.tmp", "r"); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(null_filename)
{ void *f = (void *)(ULONG_PTR)1; int r; tc_iph_count = 0; r = p_fopen_s(&f, NULL, "r"); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(null_mode)
{ void *f = (void *)(ULONG_PTR)1; int r; tc_iph_count = 0; r = p_fopen_s(&f, "tc_sec_neg_fopen.tmp", NULL); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(missing_file_read_enoent)
{
    void *f = (void *)(ULONG_PTR)1; int r;
    DeleteFileA("tc_sec_neg_fopen.tmp");
    r = p_fopen_s(&f, "tc_sec_neg_fopen.tmp", "r");
    CHECK_EQ_I(r, 2); CHECK(f == NULL);
    CHECK_EQ_I(GetFileAttributesA("tc_sec_neg_fopen.tmp") == INVALID_FILE_ATTRIBUTES, 1);
}
TC_CASE(invalid_mode_char)
{
    void *f = (void *)(ULONG_PTR)1; int r; tc_iph_count = 0;
    r = p_fopen_s(&f, "tc_sec_neg_fopen.tmp", "z"); CHECK_EQ_I(r, 22);
    CHECK(GetFileAttributesA("tc_sec_neg_fopen.tmp") == INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    p_fopen_s = (fopen_s_t)tc_crt_proc("fopen_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_fopen_s); if (!p_fopen_s) return tc_end();
    tc_iph_install();
    TC_RUN(null_pfile); TC_RUN(null_filename); TC_RUN(null_mode); TC_RUN(missing_file_read_enoent); TC_RUN(invalid_mode_char);
    return tc_end();
}
