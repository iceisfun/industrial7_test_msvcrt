/* TC
 * id:    env_getenv_basic
 * area:  env
 * funcs: getenv _putenv
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs getenv, _putenv
 */
#include "tc.h"
#include <stdlib.h>

static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }

TC_CASE(set_get_roundtrip)
{
    char *v;
    CHECK_EQ_I(_putenv("TCV_RT=hello"), 0);
    v = getenv("TCV_RT");
    CHECK(v != NULL);
    if (v) CHECK_STR(v, "hello");
    _putenv("TCV_RT=");
}
TC_CASE(missing_returns_null)
{
    CHECK(getenv("TCV_DEFINITELY_NOT_SET_12345") == NULL);
}
TC_CASE(returned_string_content_exact)
{
    char *v;
    _putenv("TCV_EX=abc");
    v = getenv("TCV_EX");
    CHECK(v != NULL);
    if (v) { CHECK_EQ_I(slen(v), 3); CHECK(v[0] == 'a' && v[1] == 'b' && v[2] == 'c' && v[3] == 0); }
    _putenv("TCV_EX=");
}
TC_CASE(name_is_not_prefix_matched)
{
    char *v;
    _putenv("TCVX=long");
    _putenv("TCVP=short");
    v = getenv("TCVP"); CHECK(v != NULL); if (v) CHECK_STR(v, "short");
    CHECK(getenv("TCV_P") == NULL);
    CHECK(getenv("TCVPX") == NULL);
    CHECK(getenv("TCVP2") == NULL);
    v = getenv("TCVX"); CHECK(v != NULL); if (v) CHECK_STR(v, "long");
    _putenv("TCVX="); _putenv("TCVP=");
}
TC_CASE(long_value_1000_chars)
{
    static char put[1100], want[1001];
    char *v; int i;
    for (i = 0; i < 1000; i++) want[i] = (char)('a' + i % 26);
    want[1000] = 0;
    { const char *p = "TCV_LONG="; int j = 0; while (*p) put[j++] = *p++; for (i = 0; i < 1001; i++) put[j + i] = want[i]; }
    CHECK_EQ_I(_putenv(put), 0);
    v = getenv("TCV_LONG");
    CHECK(v != NULL);
    if (v) { CHECK_EQ_I(slen(v), 1000); CHECK_STR(v, want); }
    _putenv("TCV_LONG=");
}
TC_CASE(value_with_spaces_and_punct)
{
    char *v;
    CHECK_EQ_I(_putenv("TCV_PUNCT=a b;c:d=e,f !@#$%^&*()"), 0);
    v = getenv("TCV_PUNCT");
    CHECK(v != NULL);
    if (v) CHECK_STR(v, "a b;c:d=e,f !@#$%^&*()");
    _putenv("TCV_PUNCT=");
}

int main(void)
{
    tc_begin();
    TC_RUN(set_get_roundtrip); TC_RUN(missing_returns_null); TC_RUN(returned_string_content_exact);
    TC_RUN(name_is_not_prefix_matched); TC_RUN(long_value_1000_chars); TC_RUN(value_with_spaces_and_punct);
    return tc_end();
}
