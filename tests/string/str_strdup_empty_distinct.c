/* TC
 * id:    str_strdup_empty_distinct
 * area:  string
 * funcs: _strdup
 * crt:   >=140
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strdup/_wcsdup: returns a pointer to the storage location for the copied string; the copy is separate storage that must be freed
 * note:  Renamed from str_neg_strdup_huge_fails: an allocation-failure path cannot be triggered portably, so this is a positive edge test: _strdup("") returns distinct, writable storage. NULL input: str_neg_strdup_null, str_neg_strdup_null_errno_probe.
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
static const char *volatile empty = "";

TC_CASE(empty_string)
{
    char *r = _strdup(empty);
    CHECK(r != NULL);
    if (!r) return;
    CHECK(r != empty); CHECK_EQ_I(r[0], 0);
    r[0] = 'z';          /* the allocation holds at least one byte and is writable */
    CHECK_EQ_I(r[0], 'z');
    r[0] = 0;
    free(r);
}
TC_CASE(copy_is_distinct)
{
    char src[4]; char *r; strcpy(src, "ab"); { char *volatile ps = src; r = _strdup(ps); }
    CHECK(r != NULL);
    if (!r) return;
    CHECK(r != src); CHECK_STR(r, "ab");
    src[0] = 'X'; CHECK_STR(r, "ab");
    free(r);
}
int main(void) { tc_begin(); TC_RUN(empty_string); TC_RUN(copy_is_distinct); return tc_end(); }
