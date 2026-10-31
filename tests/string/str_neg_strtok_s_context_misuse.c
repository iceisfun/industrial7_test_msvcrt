/* TC
 * id:    str_neg_strtok_s_context_misuse
 * area:  string
 * funcs:
 * procs: strtok_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s/_strtok_s_l: a new string argument starts a new scan and ignores the saved context; no tokens left returns NULL; only NULL str with NULL *context (or NULL context) is a parameter error
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_DECL_PROC(char *, strtok_s, (char *, const char *, char **));

TC_CASE(exhausted_then_null)
{
    char b[8]; char *ctx = NULL; char *t; strcpy(b, "a b");
    t = tc_p_strtok_s(b, " ", &ctx); CHECK_STR(t, "a");
    t = tc_p_strtok_s(NULL, " ", &ctx); CHECK_STR(t, "b");
    errno = 0x4242; tc_iph_count = 0;
    t = tc_p_strtok_s(NULL, " ", &ctx); CHECK(t == NULL);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0x4242);
    t = tc_p_strtok_s(NULL, " ", &ctx); CHECK(t == NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(new_string_ignores_stale_ctx)
{
    char old[8], nw[8]; char *ctx = NULL; char *t; strcpy(old, "x y z"); strcpy(nw, "p q");
    t = tc_p_strtok_s(old, " ", &ctx); CHECK_STR(t, "x");
    tc_iph_count = 0;
    t = tc_p_strtok_s(nw, " ", &ctx); CHECK_STR(t, "p");
    t = tc_p_strtok_s(NULL, " ", &ctx); CHECK_STR(t, "q");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(empty_string)
{
    char b[4]; char *ctx = NULL; b[0] = 0; errno = 0x4242; tc_iph_count = 0;
    CHECK(tc_p_strtok_s(b, " ", &ctx) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0x4242);
}
TC_CASE(delimiters_only)
{
    char b[8]; char *ctx = NULL; strcpy(b, "   "); errno = 0x4242; tc_iph_count = 0;
    CHECK(tc_p_strtok_s(b, " ", &ctx) == NULL); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0x4242);
    CHECK(tc_p_strtok_s(NULL, " ", &ctx) == NULL); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(strtok_s);
    if (!tc_p_strtok_s) { tc_write("TC FAIL " TC_TEST_ID "::__import strtok_s missing\n"); return 1; }
    TC_RUN(exhausted_then_null); TC_RUN(new_string_ignores_stale_ctx); TC_RUN(empty_string); TC_RUN(delimiters_only);
    return tc_end();
}
