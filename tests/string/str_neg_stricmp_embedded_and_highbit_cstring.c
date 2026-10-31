/* TC
 * id:    str_neg_stricmp_embedded_and_highbit_cstring
 * area:  string
 * funcs: _stricmp _strnicmp
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp, _strnicmp (compare at most count chars; strings are NUL-terminated)
 */
#include "tc.h"
#include <string.h>

TC_CASE(stops_at_count)
{
    volatile const char *a = "abcX", *b = "ABCY"; volatile size_t n = 3;
    CHECK_EQ_I(_strnicmp((const char *)a, (const char *)b, n), 0);
    n = 4;
    CHECK(_strnicmp((const char *)a, (const char *)b, n) < 0);
}
TC_CASE(stops_at_nul)
{
    static const char a[] = "a\0B", b[] = "a\0C";
    const char *volatile pa = a, *volatile pb = b;
    CHECK_EQ_I(_stricmp(pa, pb), 0);
}
TC_CASE(sign)
{
    volatile const char *a = "abc", *b = "abd";
    CHECK(_strnicmp((const char *)a, (const char *)b, 3) < 0);
    CHECK(_strnicmp((const char *)b, (const char *)a, 3) > 0);
    CHECK(_stricmp((const char *)a, (const char *)b) < 0);
    CHECK(_stricmp((const char *)b, (const char *)a) > 0);
}
TC_CASE(int_max_count)
{
    volatile const char *a = "abc", *b = "ABC"; volatile size_t n = 0x7fffffff;
    CHECK_EQ_I(_strnicmp((const char *)a, (const char *)b, n), 0);
}
int main(void) { tc_begin(); TC_RUN(stops_at_count); TC_RUN(stops_at_nul); TC_RUN(sign); TC_RUN(int_max_count); return tc_end(); }
