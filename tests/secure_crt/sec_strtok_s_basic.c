/* TC
 * id:    sec_strtok_s_basic
 * area:  secure_crt
 * funcs: strtok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtok_s, _strtok_s_l, wcstok_s

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
static char buf[64];
static void set(const char *s) { int i = 0; do buf[i] = s[i]; while (s[i++]); }

TC_CASE(simple_tokens)
{
    char *ctx = NULL, *t; set("one two three");
    t = strtok_s(buf, " ", &ctx); CHECK_STR(t, "one");
    t = strtok_s(NULL, " ", &ctx); CHECK_STR(t, "two");
    t = strtok_s(NULL, " ", &ctx); CHECK_STR(t, "three");
}
TC_CASE(leading_and_consecutive_delims_skipped)
{
    char *ctx = NULL, *t; set(",,a,,,b,,");
    t = strtok_s(buf, ",", &ctx); CHECK_STR(t, "a");
    t = strtok_s(NULL, ",", &ctx); CHECK_STR(t, "b");
    t = strtok_s(NULL, ",", &ctx); CHECK(t == NULL);
}
TC_CASE(multiple_delim_chars)
{
    char *ctx = NULL, *t; set("a-b+c;d");
    t = strtok_s(buf, "-+;", &ctx); CHECK_STR(t, "a");
    t = strtok_s(NULL, "-+;", &ctx); CHECK_STR(t, "b");
    t = strtok_s(NULL, "-+;", &ctx); CHECK_STR(t, "c");
    t = strtok_s(NULL, "-+;", &ctx); CHECK_STR(t, "d");
    t = strtok_s(NULL, "-+;", &ctx); CHECK(t == NULL);
}
TC_CASE(no_delims_found_single_token)
{
    char *ctx = NULL, *t; set("abcdef");
    t = strtok_s(buf, ",", &ctx); CHECK(t == buf); CHECK_STR(t, "abcdef");
    t = strtok_s(NULL, ",", &ctx); CHECK(t == NULL);
}
TC_CASE(only_delims_returns_null)
{
    char *ctx = NULL, *t; set(",,,");
    t = strtok_s(buf, ",", &ctx); CHECK(t == NULL);
}
TC_CASE(returns_null_after_last)
{
    char *ctx = NULL, *t; set("x y");
    t = strtok_s(buf, " ", &ctx); CHECK_STR(t, "x");
    t = strtok_s(NULL, " ", &ctx); CHECK_STR(t, "y");
    t = strtok_s(NULL, " ", &ctx); CHECK(t == NULL);
    t = strtok_s(NULL, " ", &ctx); CHECK(t == NULL);
}
TC_CASE(source_modified_with_nul)
{
    char *ctx = NULL, *t; set("ab cd");
    t = strtok_s(buf, " ", &ctx); CHECK(t == buf);
    CHECK_EQ_I(buf[2], 0);
    CHECK_EQ_I(buf[3], 'c');
}
int main(void) { tc_begin(); TC_RUN(simple_tokens); TC_RUN(leading_and_consecutive_delims_skipped); TC_RUN(multiple_delim_chars);
    TC_RUN(no_delims_found_single_token); TC_RUN(only_delims_returns_null); TC_RUN(returns_null_after_last); TC_RUN(source_modified_with_nul); return tc_end(); }
