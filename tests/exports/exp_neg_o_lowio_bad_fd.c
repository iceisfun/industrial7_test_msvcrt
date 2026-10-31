/* TC
 * id:    exp_neg_o_lowio_bad_fd
 * area:  exports
 * funcs:
 * procs: _o__read _o__write _o__lseek _o__dup _o__chsize_s _o__access_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _read/_write/_lseek/_dup (EBADF, invalid parameter), _chsize_s, _access_s
 * note:  PROBE: _o_ low-level IO aliases fail on bad descriptors/paths like their underlying functions.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>

typedef int (__cdecl *read_t)(int, void *, unsigned);
typedef int (__cdecl *lseek_t)(int, long, int);
typedef int (__cdecl *dup_t)(int);
typedef int (__cdecl *chsize_s_t)(int, long long);
typedef int (__cdecl *access_s_t)(const char *, int);
static read_t p_read, p_write; static lseek_t p_lseek; static dup_t p_dup; static chsize_s_t p_chsize_s; static access_s_t p_access_s;
static volatile int badfd = -1, hugefd = 9999, mode7 = 7;
static const char *volatile missing = "no\\such\\file", *volatile cnull = NULL, *volatile dot = ".";

TC_CASE(read_bad)
{
    char b[4]; int r;
    if (!p_read) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_read(badfd, b, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; r = p_read(hugefd, b, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(write_bad)
{
    char b[4] = "x"; int r;
    if (!p_write) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_write(badfd, b, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(lseek_bad)
{
    int r;
    if (!p_lseek) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_lseek(badfd, 0, 0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dup_bad)
{
    int r;
    if (!p_dup) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_dup(badfd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(chsize_s_bad)
{
    int r;
    if (!p_chsize_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; r = p_chsize_s(badfd, 0);
    CHECK_EQ_I(r, EBADF);
}
TC_CASE(access_s_errors)
{
    int r;
    if (!p_access_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_access_s(missing, 0);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0);
    tc_iph_count = 0; r = p_access_s(cnull, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; r = p_access_s(dot, mode7);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_read = (read_t)tc_crt_proc("_o__read"); p_write = (read_t)tc_crt_proc("_o__write");
    p_lseek = (lseek_t)tc_crt_proc("_o__lseek"); p_dup = (dup_t)tc_crt_proc("_o__dup");
    p_chsize_s = (chsize_s_t)tc_crt_proc("_o__chsize_s"); p_access_s = (access_s_t)tc_crt_proc("_o__access_s");
    TC_RUN(read_bad); TC_RUN(write_bad); TC_RUN(lseek_bad); TC_RUN(dup_bad); TC_RUN(chsize_s_bad); TC_RUN(access_s_errors);
    return tc_end();
}
