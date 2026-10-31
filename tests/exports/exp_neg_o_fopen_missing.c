/* TC
 * id:    exp_neg_o_fopen_missing
 * area:  exports
 * funcs:
 * procs: _o_fopen _o_fopen_s _o_fclose
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen, fopen_s; _o_ aliases undocumented
 * note:  kind=negative conf=probe. Probing: _o_fopen/_o_fopen_s mirror fopen/fopen_s failure behaviour.
 */
#include "tc.h"
#include <errno.h>
#include <stdio.h>

typedef FILE *(__cdecl *fopen_t)(const char *, const char *);
typedef int (__cdecl *fopen_s_t)(FILE **, const char *, const char *);
typedef int (__cdecl *fclose_t)(FILE *);
static fopen_t p_fopen; static fopen_s_t p_fopen_s; static fclose_t p_fclose;
static const char *const missing = "tc_exp_neg_o_fopen_missing_nonexistent.tmp";

TC_CASE(fopen_missing_null_enoent)
{
    FILE *f;
    if (!p_fopen) TC_SKIP("not exported");
    errno = 0;
    f = p_fopen(missing, "r");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, ENOENT);
    if (f && p_fclose) p_fclose(f);
}
TC_CASE(fopen_s_null_pfile_einval)
{
    int r;
    if (!p_fopen_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0;
    r = p_fopen_s(NULL, missing, "r");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fopen_s_missing_enoent)
{
    FILE *f = (FILE *)1; int r;
    if (!p_fopen_s) TC_SKIP("not exported");
    r = p_fopen_s(&f, missing, "r");
    CHECK_EQ_I(r, ENOENT);
    CHECK(f == NULL);
    if (f && f != (FILE *)1 && p_fclose) p_fclose(f);
}
int main(void)
{
    tc_begin();
    p_fopen = (fopen_t)tc_crt_proc("_o_fopen"); p_fopen_s = (fopen_s_t)tc_crt_proc("_o_fopen_s");
    p_fclose = (fclose_t)tc_crt_proc("_o_fclose");
    TC_RUN(fopen_missing_null_enoent); TC_RUN(fopen_s_null_pfile_einval); TC_RUN(fopen_s_missing_enoent);
    return tc_end();
}
