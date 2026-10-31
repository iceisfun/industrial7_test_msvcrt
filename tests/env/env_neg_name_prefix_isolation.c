/* TC
 * id:    env_neg_name_prefix_isolation
 * area:  env
 * kind:  negative
 * funcs: getenv _wgetenv _putenv
 * procs: getenv_s _dupenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv, _putenv: the whole variable name is matched; "NAME=" deletes only NAME
 * note:  getenv_s/_dupenv_s via procs (absent from msvcrt.dll; msvcr80 import lib lacks them).
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, getenv_s, (size_t *, char *, size_t, const char *));
TC_DECL_PROC(int, _dupenv_s, (char **, size_t *, const char *));
static volatile size_t vsz = 16;

TC_CASE(lookup_prefix_not_found)
{
    CHECK_EQ_I(_putenv("TC_PFXLONG=1"), 0);
    CHECK(getenv("TC_PFX") == NULL);
    CHECK(getenv("TC_PFXLONGER") == NULL);
    CHECK(_wgetenv(L"TC_PFX") == NULL);
    CHECK(_wgetenv(L"TC_PFXLONGER") == NULL);
    CHECK_STR(getenv("TC_PFXLONG"), "1");
}
TC_CASE(getenv_s_prefix_not_found)
{
    size_t r = 99; char buf[16];
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    buf[0] = 'z';
    CHECK_EQ_I(tc_p_getenv_s(&r, buf, vsz, "TC_PFX"), 0);
    CHECK_EQ_U(r, 0);
}
TC_CASE(dupenv_s_prefix_not_found)
{
    char *b = (char *)1; size_t n = 99;
    if (!tc_p__dupenv_s) TC_SKIP("_dupenv_s not exported");
    CHECK_EQ_I(tc_p__dupenv_s(&b, &n, "TC_PFX"), 0);
    CHECK(b == NULL);
    CHECK_EQ_U(n, 0);
}
TC_CASE(put_short_does_not_clobber_long)
{
    CHECK_EQ_I(_putenv("TC_PFX=s"), 0);
    CHECK_STR(getenv("TC_PFXLONG"), "1");
    CHECK_STR(getenv("TC_PFX"), "s");
}
TC_CASE(delete_short_does_not_delete_long)
{
    CHECK_EQ_I(_putenv("TC_PFX="), 0);
    CHECK_STR(getenv("TC_PFXLONG"), "1");
    CHECK(getenv("TC_PFX") == NULL);
}
TC_CASE(cleanup)
{
    _putenv("TC_PFXLONG="); _putenv("TC_PFX=");
    CHECK(getenv("TC_PFXLONG") == NULL);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(getenv_s); TC_LOAD_PROC(_dupenv_s);
    TC_RUN(lookup_prefix_not_found); TC_RUN(getenv_s_prefix_not_found); TC_RUN(dupenv_s_prefix_not_found);
    TC_RUN(put_short_does_not_clobber_long); TC_RUN(delete_short_does_not_delete_long); TC_RUN(cleanup);
    return tc_end();
}
