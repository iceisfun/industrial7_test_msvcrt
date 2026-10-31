/* TC
 * id:    str_strtok_s_basic
 * area:  string
 * funcs:
 * procs: strtok_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtok_s, _strtok_s_l
 */
#include "tc.h"

typedef char *(__cdecl *strtok_s_t)(char *, const char *, char **);
static strtok_s_t p_strtok_s;

TC_CASE(exported) { CHECK(p_strtok_s != NULL); }
TC_CASE(split)
{
    char b[] = "a,bb,ccc"; char *ctx = NULL, *t;
    if (!p_strtok_s) TC_SKIP("strtok_s not exported");
    t = p_strtok_s(b, ",", &ctx); CHECK_STR(t, "a"); CHECK_EQ_P(t, b);
    t = p_strtok_s(NULL, ",", &ctx); CHECK_STR(t, "bb");
    t = p_strtok_s(NULL, ",", &ctx); CHECK_STR(t, "ccc");
    CHECK(p_strtok_s(NULL, ",", &ctx) == NULL);
}
TC_CASE(interleaved)
{
    char a[] = "a1 a2 a3", b[] = "b1,b2,b3"; char *ca = NULL, *cb = NULL;
    if (!p_strtok_s) TC_SKIP("strtok_s not exported");
    CHECK_STR(p_strtok_s(a, " ", &ca), "a1");
    CHECK_STR(p_strtok_s(b, ",", &cb), "b1");
    CHECK_STR(p_strtok_s(NULL, " ", &ca), "a2");
    CHECK_STR(p_strtok_s(NULL, ",", &cb), "b2");
    CHECK_STR(p_strtok_s(NULL, " ", &ca), "a3");
    CHECK_STR(p_strtok_s(NULL, ",", &cb), "b3");
    CHECK(p_strtok_s(NULL, " ", &ca) == NULL);
    CHECK(p_strtok_s(NULL, ",", &cb) == NULL);
}
TC_CASE(leading_trailing)
{
    char b[] = ",,x,,y,,"; char *ctx = NULL;
    if (!p_strtok_s) TC_SKIP("strtok_s not exported");
    CHECK_STR(p_strtok_s(b, ",", &ctx), "x");
    CHECK_STR(p_strtok_s(NULL, ",", &ctx), "y");
    CHECK(p_strtok_s(NULL, ",", &ctx) == NULL);
}
TC_CASE(exhausted_repeats)
{
    char b[] = "x y"; char *ctx = NULL;
    if (!p_strtok_s) TC_SKIP("strtok_s not exported");
    CHECK_STR(p_strtok_s(b, " ", &ctx), "x");
    CHECK_STR(p_strtok_s(NULL, " ", &ctx), "y");
    CHECK(p_strtok_s(NULL, " ", &ctx) == NULL);
    CHECK(p_strtok_s(NULL, " ", &ctx) == NULL);
}

int main(void)
{
    tc_begin();
    p_strtok_s = (strtok_s_t)tc_crt_proc("strtok_s");
    TC_RUN(exported); TC_RUN(split); TC_RUN(interleaved); TC_RUN(leading_trailing); TC_RUN(exhausted_repeats);
    return tc_end();
}
