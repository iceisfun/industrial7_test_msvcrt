/* TC
 * id:    str_strnicmp_basic
 * area:  string
 * funcs: _strnicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strnicmp
 */
#include "tc.h"
#include <string.h>

static volatile const char *pa, *pb;
static int cmp(const char *a, const char *b, size_t n) { pa = a; pb = b; return _strnicmp((const char *)pa, (const char *)pb, n); }

TC_CASE(prefix_equal) { CHECK_EQ_I(cmp("HelloWorld", "hELLOthere", 5), 0); CHECK_EQ_I(cmp("abc", "ABC", 3), 0); }
TC_CASE(diff_after_n) { CHECK_EQ_I(cmp("abcX", "abcY", 3), 0); CHECK_EQ_I(cmp("abc", "abd", 0), 0); }
TC_CASE(n_longer) { CHECK_EQ_I(cmp("AbC", "aBc", 100), 0); CHECK(cmp("ab", "abc", 100) < 0); CHECK(cmp("abc", "ab", 100) > 0); }
TC_CASE(sign) { CHECK(cmp("a", "B", 1) < 0); CHECK(cmp("B", "a", 1) > 0); CHECK(cmp("Z", "a", 1) > 0); }
TC_CASE(punct_between)
{
    /* '[' (0x5b) is not a letter so unchanged; 'A' folds to 'a' (0x61) > 0x5b */
    CHECK(cmp("[", "A", 1) < 0);
    CHECK(cmp("[", "a", 1) < 0);
    CHECK(cmp("A", "[", 1) > 0);
    CHECK(cmp("_", "a", 1) < 0);
    CHECK(cmp("_", "A", 1) < 0);
}
int main(void) { tc_begin(); TC_RUN(prefix_equal); TC_RUN(diff_after_n); TC_RUN(n_longer); TC_RUN(sign); TC_RUN(punct_between); return tc_end(); }
