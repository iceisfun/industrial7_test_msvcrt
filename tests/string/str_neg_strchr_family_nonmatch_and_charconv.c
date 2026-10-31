/* TC
 * id:    str_neg_strchr_family_nonmatch_and_charconv
 * area:  string
 * funcs: strchr strrchr memchr strstr
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.1, 7.24.5.2, 7.24.5.5, 7.24.5.7 (c is converted to char; terminating NUL is part of the string); MS docs strchr, strrchr, strstr, memchr
 */
#include "tc.h"
#include <string.h>

static const char *volatile abc = "abc";
static const char *volatile empty = "";

TC_CASE(strchr_conv)
{
    const char *s = abc; volatile int c = 'a' + 256;
    CHECK_EQ_P(strchr(s, c), s);
    c = 'd'; CHECK_EQ_P(strchr(s, c), NULL);
    c = 0; CHECK_EQ_P(strchr(s, c), s + 3);
}
TC_CASE(strrchr_nul_and_miss)
{
    const char *s = abc; volatile int c = 0;
    CHECK_EQ_P(strrchr(s, c), s + 3);
    { static const char *volatile t = "abcabc"; volatile int z = 'z'; CHECK_EQ_P(strrchr(t, z), NULL); }
}
TC_CASE(empty_string)
{
    const char *s = empty; volatile int c = 'a';
    CHECK_EQ_P(strchr(s, c), NULL);
    c = 0; CHECK_EQ_P(strchr(s, c), s);
}
TC_CASE(memchr_bounds)
{
    const char *s = abc; volatile int c = 'c'; volatile size_t n = 2;
    CHECK_EQ_P(memchr(s, c, n), NULL);
    n = 3; CHECK_EQ_P(memchr(s, c, n), s + 2);
    c = 'a' + 256; CHECK_EQ_P(memchr(s, c, n), s);
    n = 0; c = 'a'; CHECK_EQ_P(memchr(s, c, n), NULL);
}
TC_CASE(strstr_nonmatch)
{
    const char *s = abc, *e = empty;
    static const char *volatile longer = "abcd", *volatile a = "a", *volatile nul = "";
    CHECK_EQ_P(strstr(s, longer), NULL);
    CHECK_EQ_P(strstr(e, a), NULL);
    CHECK_EQ_P(strstr(s, nul), s);
}
int main(void) { tc_begin(); TC_RUN(strchr_conv); TC_RUN(strrchr_nul_and_miss); TC_RUN(empty_string); TC_RUN(memchr_bounds); TC_RUN(strstr_nonmatch); return tc_end(); }
