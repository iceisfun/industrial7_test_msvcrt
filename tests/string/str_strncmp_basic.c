/* TC
 * id:    str_strncmp_basic
 * area:  string
 * funcs: strncmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.4.4; MS docs strncmp
 */
#include "tc.h"
#include <string.h>

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
static volatile const char *va, *vb;
static int ncmp(const char *a, const char *b, size_t n) { va = a; vb = b; return SGN(strncmp((const char *)va, (const char *)vb, n)); }

TC_CASE(differ_after_n) { CHECK_EQ_I(ncmp("abcX", "abcY", 3), 0); }
TC_CASE(differ_within_n) { CHECK_EQ_I(ncmp("abX", "abY", 3), -1); CHECK_EQ_I(ncmp("abY", "abX", 3), 1); }
TC_CASE(n_larger_stops_at_nul) { CHECK_EQ_I(ncmp("abc", "abc", 100), 0); CHECK_EQ_I(ncmp("ab", "abc", 100), -1); CHECK_EQ_I(ncmp("abc", "ab", 100), 1); }
TC_CASE(equal) { CHECK_EQ_I(ncmp("hello", "hello", 5), 0); CHECK_EQ_I(ncmp("", "", 4), 0); }
TC_CASE(embedded_nul) { static const char a[] = "ab\0x", b[] = "ab\0y"; CHECK_EQ_I(ncmp(a, b, 4), 0); }

int main(void) { tc_begin(); TC_RUN(differ_after_n); TC_RUN(differ_within_n); TC_RUN(n_larger_stops_at_nul); TC_RUN(equal); TC_RUN(embedded_nul); return tc_end(); }
