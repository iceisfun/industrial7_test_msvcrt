/* TC
 * id:    env_getenv_s_msvcr80_procs
 * area:  env
 * funcs: getenv _putenv
 * procs: getenv_s _putenv_s _dupenv_s
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs getenv_s, _putenv_s, _dupenv_s (CRT 8.0 / VS2005 introduced the _s variants)
 * note:  msvcr80's mingw import lib lacks the secure env functions, so they are resolved at run time.
 *        The invalid-parameter handler is installed first so the NULL-buffer case returns EINVAL instead of aborting.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(errno_t, getenv_s, (size_t *, char *, size_t, const char *));
TC_DECL_PROC(errno_t, _putenv_s, (const char *, const char *));
TC_DECL_PROC(errno_t, _dupenv_s, (char **, size_t *, const char *));

TC_CASE(getenv_s_found)
{
    char buf[64]; size_t n = 0; errno_t r;
    if (!tc_p_getenv_s || !tc_p__putenv_s) TC_SKIP("not exported");
    CHECK_EQ_I(_putenv("TC_ENV_A=hello"), 0);
    r = tc_p_getenv_s(&n, buf, sizeof buf, "TC_ENV_A");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(n, 6);   /* includes the terminating NUL */
    CHECK_STR(buf, "hello");
    _putenv("TC_ENV_A=");
}
TC_CASE(putenv_s_roundtrip)
{
    char buf[64]; size_t n = 0;
    if (!tc_p_getenv_s || !tc_p__putenv_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__putenv_s("TC_ENV_B", "value1"), 0);
    CHECK_STR(getenv("TC_ENV_B"), "value1");
    CHECK_EQ_I(tc_p_getenv_s(&n, buf, sizeof buf, "TC_ENV_B"), 0);
    CHECK_STR(buf, "value1");
    CHECK_EQ_I(tc_p__putenv_s("TC_ENV_B", ""), 0);   /* empty value removes */
    CHECK(getenv("TC_ENV_B") == NULL);
}
TC_CASE(dupenv_s_found)
{
    char *p = NULL; size_t n = 0;
    if (!tc_p__dupenv_s) TC_SKIP("not exported");
    CHECK_EQ_I(_putenv("TC_ENV_C=dupme"), 0);
    CHECK_EQ_I(tc_p__dupenv_s(&p, &n, "TC_ENV_C"), 0);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, "dupme"); CHECK_EQ_U(n, 6); free(p); }
    p = (char *)1; n = 99;
    CHECK_EQ_I(tc_p__dupenv_s(&p, &n, "TC_ENV_NOT_THERE_XYZ"), 0);   /* not found: success, buffer NULL, len 0 */
    CHECK(p == NULL);
    CHECK_EQ_U(n, 0);
    _putenv("TC_ENV_C=");
}
TC_CASE(getenv_s_null_buffer_EINVAL)
{
    char buf[8]; size_t n = 5; errno_t r;
    if (!tc_p_getenv_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = tc_p_getenv_s(&n, NULL, sizeof buf, "PATH");   /* docs: buffer NULL with sizeInBytes > 0 -> EINVAL */
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(getenv_s); TC_LOAD_PROC(_putenv_s); TC_LOAD_PROC(_dupenv_s);
    TC_RUN(getenv_s_found); TC_RUN(putenv_s_roundtrip); TC_RUN(dupenv_s_found);
    TC_RUN(getenv_s_null_buffer_EINVAL);
    return tc_end();
}
