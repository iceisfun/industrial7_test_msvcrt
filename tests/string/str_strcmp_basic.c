/* TC
 * id:    str_strcmp_basic
 * area:  string
 * funcs: strcmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.4.2; MS docs strcmp
 * note:  assert sign only
 */
#include "tc.h"
#include <string.h>

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
static volatile const char *va, *vb;
static int cmp(const char *a, const char *b) { va = a; vb = b; return SGN(strcmp((const char *)va, (const char *)vb)); }

TC_CASE(equal) { CHECK_EQ_I(cmp("abc", "abc"), 0); CHECK_EQ_I(cmp("", ""), 0); }
TC_CASE(sign) { CHECK_EQ_I(cmp("abc", "abd"), -1); CHECK_EQ_I(cmp("abd", "abc"), 1); CHECK_EQ_I(cmp("b", "a"), 1); }
TC_CASE(prefix_shorter_less) { CHECK_EQ_I(cmp("ab", "abc"), -1); CHECK_EQ_I(cmp("abc", "ab"), 1); }
TC_CASE(empty_vs_nonempty) { CHECK_EQ_I(cmp("", "a"), -1); CHECK_EQ_I(cmp("a", ""), 1); }
TC_CASE(embedded_nul) { static const char a[] = "ab\0x", b[] = "ab\0y"; CHECK_EQ_I(cmp(a, b), 0); }

int main(void) { tc_begin(); TC_RUN(equal); TC_RUN(sign); TC_RUN(prefix_shorter_less); TC_RUN(empty_vs_nonempty); TC_RUN(embedded_nul); return tc_end(); }
