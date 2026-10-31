/* TC
 * id:    wcs_neg_wcsdup_probe_empty_and_free
 * area:  wstring
 * funcs: _wcsdup free
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strdup, _wcsdup: allocates storage with malloc; the caller frees it
 * note:  Negative angle: the duplicate must not alias the source, even for an empty string.
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>

static wchar_t *volatile vsrc;
static wchar_t srcbuf[4] = { 0 };

TC_CASE(empty_non_null)
{
    wchar_t *p = _wcsdup(L"");
    CHECK(p != NULL); if (p) { CHECK_EQ_I(p[0], 0); free(p); }
}
TC_CASE(distinct_pointer)
{
    wchar_t *p; vsrc = srcbuf; p = _wcsdup(vsrc);
    CHECK(p != NULL); CHECK(p != srcbuf); free(p);
}
TC_CASE(writable_independent_copy)
{
    wchar_t *p; vsrc = srcbuf; p = _wcsdup(vsrc);
    if (!p) { CHECK(p != NULL); return; }
    p[0] = L'Z'; p[1] = 0;
    CHECK_EQ_I(srcbuf[0], 0);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(empty_non_null); TC_RUN(distinct_pointer); TC_RUN(writable_independent_copy); return tc_end(); }
