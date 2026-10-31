/* TC
 * id:    str_strdup_basic
 * area:  string
 * funcs: _strdup free
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strdup
 * note:  free() is resolved from the same CRT as _strdup (import table checked).
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static const char *volatile v_src;

TC_CASE(copy_distinct)
{
    static char src[] = "hello world"; char *d;
    v_src = src; d = _strdup((const char *)v_src);
    CHECK(d != NULL);
    if (d) { CHECK(d != src); CHECK_STR(d, src); free(d); }
}
TC_CASE(empty)
{
    char *d; v_src = "";
    d = _strdup((const char *)v_src);
    CHECK(d != NULL);
    if (d) { CHECK_EQ_I(d[0], 0); free(d); }
}
TC_CASE(independent)
{
    char src[] = "abc"; char *d;
    v_src = src; d = _strdup((const char *)v_src);
    CHECK(d != NULL);
    if (d) { d[0] = 'X'; CHECK_STR(src, "abc"); CHECK_STR(d, "Xbc"); src[1] = 'Y'; CHECK_STR(d, "Xbc"); free(d); }
}
int main(void) { tc_begin(); TC_RUN(copy_distinct); TC_RUN(empty); TC_RUN(independent); return tc_end(); }
