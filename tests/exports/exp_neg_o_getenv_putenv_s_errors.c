/* TC
 * id:    exp_neg_o_getenv_putenv_s_errors
 * area:  exports
 * funcs:
 * procs: _o_getenv_s _o__putenv_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs getenv_s, _putenv_s (EINVAL, ERANGE, required size in pReturnValue)
 * note:  PROBE: _o_getenv_s/_o__putenv_s aliases validate args like getenv_s/_putenv_s.
 *        The _o_ aliases are undocumented; expectations mirror the documented behaviour of the underlying function.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>

typedef int (__cdecl *getenv_s_t)(size_t *, char *, size_t, const char *);
typedef int (__cdecl *putenv_s_t)(const char *, const char *);
static getenv_s_t p_getenv_s; static putenv_s_t p_putenv_s;
static const char *volatile nm = "TC_EXP_NEG_ENV", *volatile nmundef = "TC_EXP_NEG_UNDEFINED_VAR", *volatile cnull = NULL;
static const char *volatile pathn = "PATH", *volatile eqname = "A=B", *volatile xval = "x";

TC_CASE(name_null)
{
    char buf[8]; size_t n = 99; int r;
    if (!p_getenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0;
    r = p_getenv_s(&n, buf, 8, cnull);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(buf_null_with_size)
{
    size_t n = 99; int r;
    if (!p_getenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0;
    r = p_getenv_s(&n, NULL, 8, pathn);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(small_buffer_erange)
{
    char buf[8]; size_t n = 99; int r;
    if (!p_getenv_s || !p_putenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    CHECK_EQ_I(p_putenv_s(nm, "hello"), 0);
    tc_iph_count = 0;
    r = p_getenv_s(&n, buf, 1, nm);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_U(n, 6);
    p_putenv_s(nm, "");
}
TC_CASE(undefined_var)
{
    char buf[8]; size_t n = 99; int r;
    if (!p_getenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    buf[0] = 'Z'; tc_iph_count = 0;
    r = p_getenv_s(&n, buf, 8, nmundef);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 0); CHECK_EQ_I(buf[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(putenv_s_bad)
{
    int r;
    if (!p_putenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0;
    r = p_putenv_s(cnull, xval);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(putenv_s_name_with_equals)  /* isolated guess: '=' in name rejected with EINVAL (handler count not asserted) */
{
    int r;
    if (!p_putenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    r = p_putenv_s(eqname, xval);
    CHECK_EQ_I(r, EINVAL);
}
int main(void)
{
    tc_begin();
    p_getenv_s = (getenv_s_t)tc_crt_proc("_o_getenv_s"); p_putenv_s = (putenv_s_t)tc_crt_proc("_o__putenv_s");
    TC_RUN(name_null); TC_RUN(buf_null_with_size); TC_RUN(small_buffer_erange); TC_RUN(undefined_var); TC_RUN(putenv_s_bad); TC_RUN(putenv_s_name_with_equals);
    return tc_end();
}
