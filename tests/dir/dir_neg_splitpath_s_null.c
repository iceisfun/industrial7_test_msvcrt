/* TC
 * id:    dir_neg_splitpath_s_null
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _splitpath_s _wsplitpath_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _splitpath_s (error conditions table: NULL path, NULL buffer with nonzero size, non-NULL buffer with zero size -> EINVAL)
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *sp_t)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t);
typedef int (__cdecl *wsp_t)(const wchar_t *, wchar_t *, size_t, wchar_t *, size_t, wchar_t *, size_t, wchar_t *, size_t);
static sp_t sp; static wsp_t wsp;
static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

#define EXPECT_EINVAL(call) do { int r_; tc_iph_count = 0; errno = 0; r_ = (call); \
    CHECK_EQ_I(r_, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); } while (0)

TC_CASE(path_null)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!sp) TC_SKIP("n/a");
    EXPECT_EINVAL(sp(NULL, d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT));
}
TC_CASE(drive_size_zero)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!sp) TC_SKIP("n/a");
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, 0, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT));
}
TC_CASE(drive_null_size)
{
    char di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!sp) TC_SKIP("n/a");
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), NULL, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT));
}
TC_CASE(dir_mismatch)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!sp) TC_SKIP("n/a");
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, _MAX_DRIVE, NULL, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT));
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, _MAX_DRIVE, di, 0, f, _MAX_FNAME, e, _MAX_EXT));
}
TC_CASE(fname_mismatch)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!sp) TC_SKIP("n/a");
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, _MAX_DRIVE, di, _MAX_DIR, NULL, _MAX_FNAME, e, _MAX_EXT));
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, _MAX_DRIVE, di, _MAX_DIR, f, 0, e, _MAX_EXT));
}
TC_CASE(ext_mismatch)
{
    char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!sp) TC_SKIP("n/a");
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, NULL, _MAX_EXT));
    EXPECT_EINVAL(sp(vs("C:\\a\\b.c"), d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, 0));
}
TC_CASE(wide_path_null)
{
    wchar_t d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
    if (!wsp) TC_SKIP("n/a");
    EXPECT_EINVAL(wsp(NULL, d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT));
    EXPECT_EINVAL(wsp(L"C:\\a", NULL, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT));
}
int main(void)
{
    tc_begin();
    sp = (sp_t)tc_crt_proc("_splitpath_s"); wsp = (wsp_t)tc_crt_proc("_wsplitpath_s");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(path_null); TC_RUN(drive_size_zero); TC_RUN(drive_null_size); TC_RUN(dir_mismatch);
    TC_RUN(fname_mismatch); TC_RUN(ext_mismatch); TC_RUN(wide_path_null);
    return tc_end();
}
