/* TC
 * id:    env_putenv_s_basic
 * area:  env
 * funcs: getenv
 * procs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s
 * note:  _putenv_s is resolved at run time (not in every mingw import library); skipped where the DLL lacks it.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));

static const char *volatile n_a = "TCENV_PS";
static const char *volatile n_lo = "tcenv_ps";
static const char *volatile v1 = "first";
static const char *volatile v2 = "second";
static const char *volatile veq = "a=b=c";
static const char *volatile vdel = "";
static char longv[2001];

TC_CASE(add_returns_0)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK_EQ_I(tc_p__putenv_s(n_a, v1), 0);
    CHECK(getenv(n_a) != NULL);
    if (getenv(n_a)) CHECK_STR(getenv(n_a), "first");
    tc_p__putenv_s(n_a, vdel);
}
TC_CASE(replace)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK_EQ_I(tc_p__putenv_s(n_a, v1), 0);
    CHECK_EQ_I(tc_p__putenv_s(n_a, v2), 0);
    CHECK(getenv(n_a) != NULL);
    if (getenv(n_a)) CHECK_STR(getenv(n_a), "second");
    tc_p__putenv_s(n_a, vdel);
}
TC_CASE(value_with_equals_sign_ok)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK_EQ_I(tc_p__putenv_s(n_a, veq), 0);
    CHECK(getenv(n_a) != NULL);
    if (getenv(n_a)) CHECK_STR(getenv(n_a), "a=b=c");
    tc_p__putenv_s(n_a, vdel);
}
TC_CASE(case_insensitive_replace)
{
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    CHECK_EQ_I(tc_p__putenv_s(n_a, v1), 0);
    CHECK_EQ_I(tc_p__putenv_s(n_lo, v2), 0);
    CHECK(getenv(n_a) != NULL);
    if (getenv(n_a)) CHECK_STR(getenv(n_a), "second");
    tc_p__putenv_s(n_lo, vdel);
    CHECK(getenv(n_a) == NULL);
}
TC_CASE(long_value)
{
    int i;
    if (!tc_p__putenv_s) TC_SKIP("_putenv_s not exported");
    for (i = 0; i < 2000; i++) longv[i] = (char)('A' + i % 26);
    longv[2000] = 0;
    CHECK_EQ_I(tc_p__putenv_s(n_a, longv), 0);
    CHECK(getenv(n_a) != NULL);
    if (getenv(n_a)) CHECK_STR(getenv(n_a), longv);
    tc_p__putenv_s(n_a, vdel);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_putenv_s);
    TC_RUN(add_returns_0); TC_RUN(replace); TC_RUN(value_with_equals_sign_ok);
    TC_RUN(case_insensitive_replace); TC_RUN(long_value);
    return tc_end();
}
