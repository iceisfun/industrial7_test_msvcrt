/* TC
 * id:    fio_neg_wfreopen_args
 * area:  stdio_file
 * funcs: _wfreopen fopen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs freopen, _wfreopen (invalid parameter: EINVAL; fopen failure: ENOENT)
 * note:  after a failed freopen the stream state is unspecified, so _fcloseall() is used for cleanup */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static const char *vfn = "tc_fio_neg_wfreopen_args_a.tmp";
static const wchar_t *wfn = L"tc_fio_neg_wfreopen_args_a.tmp";

TC_CASE(null_stream)
{
    FILE *r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wfreopen(wfn, L"r", NULL);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(bad_mode)
{
    FILE *fp = fopen(vfn, "w"), *r;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wfreopen(wfn, L"z", fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
    _fcloseall();
    DeleteFileA(vfn);
}
TC_CASE(missing_file)
{
    FILE *fp = fopen(vfn, "w"), *r;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wfreopen(L"tc_fio_neg_wfreopen_args_missing.tmp", L"r", fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOENT);
    _fcloseall();
    DeleteFileA(vfn);
}
int main(void) { tc_begin(); TC_RUN(null_stream); TC_RUN(bad_mode); TC_RUN(missing_file); return tc_end(); }
