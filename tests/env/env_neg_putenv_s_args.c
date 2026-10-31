/* TC
 * id:    env_neg_putenv_s_args
 * area:  env
 * kind:  negative
 * funcs: getenv
 * procs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s: name or value NULL -> EINVAL (returned and set in errno); invalid-parameter handler invoked
 * note:  Only the NULL-argument cases are documented. name-with-'=' lives in env_neg_probe_putenv_s_name_equals and the
 *        empty-name case in env_neg_probe_putenv_s_empty_name (neither is tabulated in the docs).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));
static const char *volatile nm = "TCENV_PSN";
static const char *volatile nullp = NULL;
static const char *volatile val = "v";
static int have_iph;

static void run1(const char *n, const char *v, int check_iph)
{
    int r;
    tc_iph_count = 0; errno = 0;
    r = tc_p__putenv_s(n, v);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    if (have_iph && check_iph) CHECK_EQ_I(tc_iph_count, 1);
}

TC_CASE(name_NULL) { if (!tc_p__putenv_s) TC_SKIP("not exported"); run1(nullp, val, 1); }
TC_CASE(value_NULL) { if (!tc_p__putenv_s) TC_SKIP("not exported"); run1(nm, nullp, 1); CHECK(getenv(nm) == NULL); }
TC_CASE(no_variable_created_on_failure)
{
    if (!tc_p__putenv_s) TC_SKIP("not exported");
    tc_p__putenv_s(nm, nullp);
    CHECK(getenv("TCENV_PSN") == NULL);
}

int main(void)
{
    tc_begin();
    have_iph = tc_iph_install();
    TC_LOAD_PROC(_putenv_s);
    TC_RUN(name_NULL); TC_RUN(value_NULL);
    TC_RUN(no_variable_created_on_failure);
    return tc_end();
}
