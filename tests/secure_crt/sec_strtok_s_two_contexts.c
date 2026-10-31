/* TC
 * id:    sec_strtok_s_two_contexts
 * area:  secure_crt
 * funcs: strtok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtok_s: context is caller-supplied, so re-entrant

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
static char a[32], b[32];
static void set(char *d, const char *s) { int i = 0; do d[i] = s[i]; while (s[i++]); }

TC_CASE(interleaved_two_strings)
{
    char *c1 = NULL, *c2 = NULL, *t; set(a, "a1 a2 a3"); set(b, "b1,b2,b3");
    t = strtok_s(a, " ", &c1); CHECK_STR(t, "a1");
    t = strtok_s(b, ",", &c2); CHECK_STR(t, "b1");
    t = strtok_s(NULL, " ", &c1); CHECK_STR(t, "a2");
    t = strtok_s(NULL, ",", &c2); CHECK_STR(t, "b2");
    t = strtok_s(NULL, " ", &c1); CHECK_STR(t, "a3");
    t = strtok_s(NULL, ",", &c2); CHECK_STR(t, "b3");
    t = strtok_s(NULL, " ", &c1); CHECK(t == NULL);
    t = strtok_s(NULL, ",", &c2); CHECK(t == NULL);
}
TC_CASE(context_advances_per_call)
{
    char *c = NULL, *t; set(a, "x y z");
    t = strtok_s(a, " ", &c); CHECK_STR(t, "x"); CHECK(c != NULL);
    t = strtok_s(NULL, " ", &c); CHECK_STR(t, "y"); CHECK(c != NULL);
    t = strtok_s(NULL, " ", &c); CHECK_STR(t, "z");
}
TC_CASE(delims_may_change_between_calls)
{
    char *c = NULL, *t; set(a, "k=v;k2=v2");
    t = strtok_s(a, "=", &c); CHECK_STR(t, "k");
    t = strtok_s(NULL, ";", &c); CHECK_STR(t, "v");
    t = strtok_s(NULL, "=", &c); CHECK_STR(t, "k2");
    t = strtok_s(NULL, ";", &c); CHECK_STR(t, "v2");
}
int main(void) { tc_begin(); TC_RUN(interleaved_two_strings); TC_RUN(context_advances_per_call); TC_RUN(delims_may_change_between_calls); return tc_end(); }
