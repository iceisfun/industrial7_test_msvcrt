/* TC
 * id:    pfx_neg_fopen_bad_mode
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (invalid mode / NULL filename or mode -> invalid parameter handler, errno EINVAL, returns NULL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile path = "tc_pfx_neg_fopen_bad_mode_a.tmp";
static const char *volatile m_empty = "";
static const char *volatile m_z = "z";
static const char *volatile m_null = NULL;
static const char *volatile p_null = NULL;
static const char *volatile m_w = "w";

static int exists(void) { return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES; }

TC_CASE(empty_mode)
{
    FILE *f; tc_iph_count = 0; errno = 0;
    f = fopen(path, m_empty);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f);
}
TC_CASE(unknown_mode)
{
    FILE *f; tc_iph_count = 0; errno = 0;
    f = fopen(path, m_z);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f);
}
TC_CASE(null_filename)
{
    FILE *f; tc_iph_count = 0; errno = 0;
    f = fopen(p_null, m_w);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f);
}
TC_CASE(null_mode)
{
    FILE *f; tc_iph_count = 0; errno = 0;
    f = fopen(path, m_null);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) fclose(f);
}
TC_CASE(no_file_created) { CHECK(!exists()); if (exists()) DeleteFileA(path); }

int main(void)
{
    tc_begin();
    DeleteFileA(path);
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(empty_mode); TC_RUN(unknown_mode); TC_RUN(null_filename); TC_RUN(null_mode); TC_RUN(no_file_created);
    return tc_end();
}
