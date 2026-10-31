/* TC
 * id:    wcs_neg_wcsnlen_null
 * area:  wstring
 * funcs:
 * procs: wcsnlen
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strnlen, strnlen_s, wcsnlen, wcsnlen_s
 * note:  PROBES whether the exported wcsnlen tolerates a NULL pointer. The docs promise 0-for-NULL only for the _s variants
 *        (which are inline wrappers in the UCRT headers); the plain export may simply dereference. Each call runs in a child
 *        process so a crash is recorded as an exit code (0xC0000005) rather than killing the test. Most likely values asserted:
 *        (NULL,0) returns 0 without crashing (loop tests the count first); (NULL,10) dereferences NULL and dies with
 *        STATUS_ACCESS_VIOLATION (0xC0000005) since the plain export has no NULL check (unlike wcsnlen_s).
 */
#include "tc.h"
#include <wchar.h>

typedef size_t (__cdecl *wcsnlen_t)(const wchar_t *, size_t);

TC_CASE(null_zero)
{
    DWORD code = 99; int ran = tc_spawn_self("null0", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
TC_CASE(null_ten)
{
    DWORD code = 99; int ran = tc_spawn_self("null10", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000005u);
}

int main(void)
{
    const char *c; wcsnlen_t p;
    tc_begin();
    p = (wcsnlen_t)tc_crt_proc("wcsnlen");
    if ((c = tc_child_name()) != NULL) {
        volatile size_t n;
        if (!p) return 78;
        n = p(NULL, c[4] == '0' && c[5] == 0 ? 0 : 10);
        return n == 0 ? 0 : 77;
    }
    TC_RUN(null_zero); TC_RUN(null_ten);
    return tc_end();
}
