/* TC
 * id:    pfx_neg_fopen_s_bad_args
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: fopen_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen_s, _wfopen_s (EINVAL for NULL pFile/filename/mode, invalid mode; *pFile set to NULL on error)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static const char *volatile path = "tc_pfx_neg_fopen_s_a.tmp";
static const char *volatile nofile = "tc_pfx_neg_fopen_s_nofile.tmp";
static const char *volatile p_null = NULL;
static const char *volatile m_bad = "q";

TC_CASE(null_pfile)
{
    int r; if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    tc_iph_count = 0; r = tc_p_fopen_s(NULL, path, "w");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES);
}
TC_CASE(null_filename)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1234; int r; if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    tc_iph_count = 0; r = tc_p_fopen_s(&f, p_null, "w");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(bad_mode)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1234; int r; if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    tc_iph_count = 0; r = tc_p_fopen_s(&f, path, m_bad);
    CHECK_EQ_I(r, EINVAL); CHECK(f == NULL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES);
}
TC_CASE(missing_file)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1234; int r; if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, nofile, "r");
    CHECK_EQ_I(r, ENOENT); CHECK(f == NULL);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(fopen_s);
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(null_pfile); TC_RUN(null_filename); TC_RUN(bad_mode); TC_RUN(missing_file);
    return tc_end();
}
