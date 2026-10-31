/* TC
 * id:    gsm_stricmp_strnicmp_basic
 * area:  string
 * funcs: _stricmp _strnicmp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stricmp, _strnicmp (compare lowercase-converted strings; return <0, 0, >0; only the sign is specified)
 */
#include "tc.h"
#include <string.h>

static const char *volatile A, *volatile B;
static int ic(const char *a, const char *b) { A = a; B = b; return _stricmp((const char *)A, (const char *)B); }
static int nic(const char *a, const char *b, size_t n) { A = a; B = b; return _strnicmp((const char *)A, (const char *)B, n); }

TC_CASE(equal) { CHECK_EQ_I(ic("Hello", "hELLO"), 0); CHECK_EQ_I(ic("abc123_", "ABC123_"), 0); }
TC_CASE(sign)
{
    CHECK(ic("apple", "Banana") < 0); CHECK(ic("Banana", "apple") > 0);
    CHECK(ic("ABC", "abd") < 0); CHECK(ic("abd", "ABC") > 0);
}
TC_CASE(prefix) { CHECK(ic("abc", "ABCD") < 0); CHECK(ic("ABCD", "abc") > 0); CHECK(ic("", "a") < 0); CHECK(ic("a", "") > 0); }
TC_CASE(n_limit)
{
    CHECK_EQ_I(nic("abcdef", "ABCxyz", 3), 0);
    CHECK(nic("abcdef", "ABCxyz", 4) < 0); CHECK(nic("ABCxyz", "abcdef", 4) > 0);
}
TC_CASE(n_zero) { CHECK_EQ_I(nic("a", "b", 0), 0); CHECK_EQ_I(nic("", "x", 0), 0); }
TC_CASE(n_large) { CHECK_EQ_I(nic("AbC", "aBc", 100), 0); CHECK(nic("ab", "ABC", 100) < 0); }
TC_CASE(lower_fold)
{
    /* comparison is done after conversion to LOWERCASE: '_'(0x5F) < 'a'(0x61), '['(0x5B) < 'a'. (uppercase folding would give the opposite) */
    CHECK(ic("_", "a") < 0); CHECK(ic("a", "_") > 0);
    CHECK(ic("_", "A") < 0); CHECK(ic("A", "_") > 0);
    CHECK(ic("[", "A") < 0);
    CHECK(nic("_x", "AX", 2) < 0);
}
TC_CASE(empty) { CHECK_EQ_I(ic("", ""), 0); CHECK_EQ_I(nic("", "", 5), 0); }
int main(void)
{
    tc_begin(); TC_RUN(equal); TC_RUN(sign); TC_RUN(prefix); TC_RUN(n_limit); TC_RUN(n_zero); TC_RUN(n_large); TC_RUN(lower_fold); TC_RUN(empty);
    return tc_end();
}
