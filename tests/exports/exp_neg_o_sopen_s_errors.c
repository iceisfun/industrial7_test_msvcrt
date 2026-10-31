/* TC
 * id:    exp_neg_o_sopen_s_errors
 * area:  exports
 * funcs:
 * procs: _o__sopen_s _o__wfopen_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _sopen_s, _wfopen_s (ENOENT, EINVAL, fd -1 / fp NULL on failure)
 * note:  PROBE: _o_ open aliases fail on NULL out-pointers, missing files, bad share flag and bad mode strings.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <fcntl.h>
#include <share.h>
#include <stdio.h>

typedef int (__cdecl *sopen_s_t)(int *, const char *, int, int, int);
typedef int (__cdecl *wfopen_s_t)(FILE **, const wchar_t *, const wchar_t *);
static sopen_s_t p_sopen_s; static wfopen_s_t p_wfopen_s;
static volatile int bad_sh = 0x7777;
static const char *volatile miss = "missing\\x_tc_exp_neg";
static const wchar_t *volatile wmiss = L"missing\\x_tc_exp_neg", *volatile wx = L"tc_exp_neg_x.tmp", *volatile wzz = L"zz";

TC_CASE(null_fd_out)
{
    int r;
    if (!p_sopen_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; r = p_sopen_s(NULL, miss, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(missing_file)
{
    int fd = 123, r;
    if (!p_sopen_s) TC_SKIP("not exported");
    r = p_sopen_s(&fd, miss, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(fd, -1);
}
TC_CASE(bad_shflag)
{
    int fd = 123, r;
    if (!p_sopen_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; r = p_sopen_s(&fd, miss, _O_RDONLY, bad_sh, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(fd, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wfopen_missing)
{
    FILE *fp = (FILE *)(ULONG_PTR)1; int r;
    if (!p_wfopen_s) TC_SKIP("not exported");
    r = p_wfopen_s(&fp, wmiss, L"r");
    CHECK_EQ_I(r, ENOENT); CHECK(fp == NULL);
}
TC_CASE(wfopen_bad_mode)
{
    FILE *fp = (FILE *)(ULONG_PTR)1; int r;
    if (!p_wfopen_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; r = p_wfopen_s(&fp, wx, wzz);
    CHECK_EQ_I(r, EINVAL); CHECK(fp == NULL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_sopen_s = (sopen_s_t)tc_crt_proc("_o__sopen_s"); p_wfopen_s = (wfopen_s_t)tc_crt_proc("_o__wfopen_s");
    TC_RUN(null_fd_out); TC_RUN(missing_file); TC_RUN(bad_shflag); TC_RUN(wfopen_missing); TC_RUN(wfopen_bad_mode);
    return tc_end();
}
