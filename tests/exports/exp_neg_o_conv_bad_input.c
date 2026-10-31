/* TC
 * id:    exp_neg_o_conv_bad_input
 * area:  exports
 * funcs:
 * procs: _o_strtol _o_strtoul _o_atoi _o__wtoi _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtol/strtoul (EINVAL for bad base, ERANGE), atoi (invalid parameter on NULL)
 * note:  PROBE: _o_ conversion aliases validate base/NULL like strtol/atoi.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>

typedef long (__cdecl *strtol_t)(const char *, char **, int);
typedef unsigned long (__cdecl *strtoul_t)(const char *, char **, int);
typedef int (__cdecl *atoi_t)(const char *);
typedef int (__cdecl *wtoi_t)(const wchar_t *);
static strtol_t p_strtol; static strtoul_t p_strtoul; static atoi_t p_atoi; static wtoi_t p_wtoi;
static const char *volatile s12 = "12", *volatile sbig = "99999999999999999999", *volatile sneg = "-99999999999999999999";
static const char *volatile snull = NULL; static const wchar_t *volatile wnull = NULL;

TC_CASE(bad_base)
{
    char *e = NULL; long r;
    if (!p_strtol) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_strtol(s12, &e, 1);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; r = p_strtol(s12, &e, 37);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_string)
{
    long r;
    if (!p_strtol) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_strtol(snull, NULL, 10);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(strtoul_overflow)
{
    unsigned long r;
    if (!p_strtoul) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_strtoul(sbig, NULL, 10);
    CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(strtol_overflow)
{
    long r;
    if (!p_strtol) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_strtol(sbig, NULL, 10);
    CHECK_EQ_I(r, LONG_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0; r = p_strtol(sneg, NULL, 10);
    CHECK_EQ_I(r, LONG_MIN); CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(atoi_null)
{
    int r;
    if (!p_atoi) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_atoi(snull);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wtoi_null)
{
    int r;
    if (!p_wtoi) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0; r = p_wtoi(wnull);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_strtol = (strtol_t)tc_crt_proc("_o_strtol"); p_strtoul = (strtoul_t)tc_crt_proc("_o_strtoul");
    p_atoi = (atoi_t)tc_crt_proc("_o_atoi"); p_wtoi = (wtoi_t)tc_crt_proc("_o__wtoi");
    TC_RUN(bad_base); TC_RUN(null_string); TC_RUN(strtoul_overflow); TC_RUN(strtol_overflow); TC_RUN(atoi_null); TC_RUN(wtoi_null);
    return tc_end();
}
