/* TC
 * id:    fio_neg_probe_wfopen_s_null_pfile_state
 * area:  stdio_file
 * funcs:
 * procs: _wfopen_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen_s, _wfopen_s (error-condition table)
 * note:  Probes the contents of *pFile after _wfopen_s fails parameter validation (NULL filename / NULL mode).
 *        The docs table says "unchanged"; the implementation is believed to store NULL into *pFile before validating the
 *        remaining arguments. This test asserts NULL (most likely); a failure here means the docs table is literally right.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *wfopen_s_t)(FILE **, const wchar_t *, const wchar_t *);
static wfopen_s_t p;

TC_CASE(null_name)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; const wchar_t *volatile name = NULL; tc_iph_count = 0; if (!p) TC_SKIP("not exported");
    r = p(&f, name, L"r");
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK(f == NULL);
}
TC_CASE(null_mode)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; const wchar_t *volatile mode = NULL; tc_iph_count = 0; if (!p) TC_SKIP("not exported");
    r = p(&f, L"tc_fio_neg_probe_wfopen_s_x.tmp", mode);
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK(f == NULL);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p = (wfopen_s_t)tc_crt_proc("_wfopen_s");
    TC_RUN(null_name); TC_RUN(null_mode);
    return tc_end();
}
