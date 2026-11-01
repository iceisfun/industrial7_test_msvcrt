/* TC
 * id:    ucr_neg_o_fopen_missing
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _o_fopen _o_fclose
 * crt:   win10
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen (ENOENT for missing file; EINVAL + invalid parameter handler for NULL filename / invalid mode)
 * note:  DUAL STATE (Win10 22H2 19045 ucrtbase; analysis: every _o_ export runs inside __crt_state_management::wrapped_invoke, which switches to the
 *        separate OS-state errno and invalid-parameter handler; audit probe: _o__read(9999) -> app errno 0 / *_o__errno()=9 / OS handler 1). So the APP-state
 *        errno and handler (errno, _set_invalid_parameter_handler) stay untouched (asserted == 0 here) while the error contract is visible in the OS state
 *        (_o__errno, _o__set_invalid_parameter_handler), which is asserted with the plain-function values. Deterministic.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef FILE *(__cdecl *fopen_t)(const char *, const char *);
typedef int (__cdecl *fclose_t)(FILE *);
static fopen_t p_fopen; static fclose_t p_fclose;

TC_CASE(missing)
{
    FILE *f; errno = 0; tc_iph_count = 0; tc_o_reset();
    f = p_fopen("tc_ucr_neg_o_fopen_missing_nonexistent.tmp", "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_o_errno(), ENOENT); CHECK_EQ_I(tc_oiph_count, 0);
    if (f) p_fclose(f);
}
TC_CASE(null_name)
{
    FILE *f; errno = 0; tc_iph_count = 0; tc_o_reset(); f = p_fopen(NULL, "r");
    CHECK(f == NULL); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_o_errno(), EINVAL); CHECK_EQ_I(tc_oiph_count, 1);
    if (f) p_fclose(f);
}
TC_CASE(bad_mode)
{
    FILE *f; errno = 0; tc_iph_count = 0; tc_o_reset(); f = p_fopen("tc_ucr_neg_o_fopen_missing_x.tmp", "zz");
    CHECK(f == NULL); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_o_errno(), EINVAL); CHECK_EQ_I(tc_oiph_count, 1);
    if (f) { p_fclose(f); remove("tc_ucr_neg_o_fopen_missing_x.tmp"); }
}
int main(void)
{
    tc_begin();
    p_fopen = (fopen_t)tc_crt_proc("_o_fopen"); p_fclose = (fclose_t)tc_crt_proc("_o_fclose");
    if (!p_fopen || !p_fclose || !(tc_iph_install() && tc_oiph_install())) { tc_write("TC FAIL missing proc\n"); return 1; }
    TC_RUN(missing); TC_RUN(null_name); TC_RUN(bad_mode);
    return tc_end();
}
