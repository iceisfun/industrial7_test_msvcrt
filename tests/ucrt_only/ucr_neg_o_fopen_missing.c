/* TC
 * id:    ucr_neg_o_fopen_missing
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _o_fopen _o_fclose
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen (ENOENT for missing file; EINVAL + invalid parameter handler for NULL filename / invalid mode)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef FILE *(__cdecl *fopen_t)(const char *, const char *);
typedef int (__cdecl *fclose_t)(FILE *);
static fopen_t p_fopen; static fclose_t p_fclose;

TC_CASE(missing)
{
    FILE *f; errno = 0; tc_iph_count = 0;
    f = p_fopen("tc_ucr_neg_o_fopen_missing_nonexistent.tmp", "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, 0);
    if (f) p_fclose(f);
}
TC_CASE(null_name)
{
    FILE *f; errno = 0; tc_iph_count = 0; f = p_fopen(NULL, "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) p_fclose(f);
}
TC_CASE(bad_mode)
{
    FILE *f; errno = 0; tc_iph_count = 0; f = p_fopen("tc_ucr_neg_o_fopen_missing_x.tmp", "zz");
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (f) { p_fclose(f); remove("tc_ucr_neg_o_fopen_missing_x.tmp"); }
}
int main(void)
{
    tc_begin();
    p_fopen = (fopen_t)tc_crt_proc("_o_fopen"); p_fclose = (fclose_t)tc_crt_proc("_o_fclose");
    if (!p_fopen || !p_fclose || !tc_iph_install()) { tc_write("TC FAIL missing proc\n"); return 1; }
    TC_RUN(missing); TC_RUN(null_name); TC_RUN(bad_mode);
    return tc_end();
}
