/* TC
 * id:    io_neg_stat64_errors
 * area:  lowio
 * funcs: _stat64 _wstat64 _set_invalid_parameter_handler
 * procs: _stat64i32 _wstat64i32
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stat, _wstat (return -1, errno ENOENT / EINVAL); Parameter validation
 * note:  the empty-string path is in io_neg_stat64_empty_path_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define MISSING "tc_io_neg_stat64_errors_missing.tmp"
#define WMISSING L"tc_io_neg_stat64_errors_missing.tmp"
TC_DECL_PROC(int, _stat64i32, (const char *, struct _stat64i32 *));
TC_DECL_PROC(int, _wstat64i32, (const wchar_t *, struct _stat64i32 *));

TC_CASE(missing)
{
    struct _stat64 st; errno = 0;
    CHECK_EQ_I(_stat64(MISSING, &st), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(null_path)
{
    struct _stat64 st; const char *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_stat64(p, &st), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf)
{
    struct _stat64 *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_stat64(".", p), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide)
{
    struct _stat64 st; const wchar_t *volatile np = NULL; struct _stat64 *volatile nb = NULL;
    CHECK(tc_iph_install());
    errno = 0;
    CHECK_EQ_I(_wstat64(WMISSING, &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_wstat64(np, &st), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_wstat64(L".", nb), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(i32_variants)
{
    struct _stat64i32 st; const char *volatile np = NULL; const wchar_t *volatile wnp = NULL;
    if (!tc_p__stat64i32 || !tc_p__wstat64i32) TC_SKIP("i32 variants not exported");
    CHECK(tc_iph_install());
    errno = 0;
    CHECK_EQ_I(tc_p__stat64i32(MISSING, &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    errno = 0;
    CHECK_EQ_I(tc_p__wstat64i32(WMISSING, &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__stat64i32(np, &st), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__wstat64i32(wnp, &st), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_stat64i32); TC_LOAD_PROC(_wstat64i32);
    TC_RUN(missing); TC_RUN(null_path); TC_RUN(null_buf); TC_RUN(wide); TC_RUN(i32_variants);
    return tc_end();
}
