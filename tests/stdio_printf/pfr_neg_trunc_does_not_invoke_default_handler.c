/* TC
 * id:    pfr_neg_trunc_does_not_invoke_default_handler
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf, snprintf (truncation returns a value; not a parameter error)
 * note:  Child-process test with the DEFAULT handler: plain truncation must not terminate the process (exit 77 expected).
 *        snprintf is exercised only on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(legacy_trunc_survives) { DWORD c = 0; CHECK(tc_spawn_self("legacy", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(c99_trunc_survives)
{
#ifdef TC_UCRT
    DWORD c = 0; CHECK(tc_spawn_self("c99", &c, 20000)); CHECK_EQ_U(c, 77);
#else
    TC_SKIP("snprintf C99 only checked on ucrtbase");
#endif
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[8];
#ifdef TC_UCRT
        if (c[0] == 'c') { snprintf(b, 4, "%s", "abcdefgh"); return 77; }
#endif
        _snprintf(b, 4, "%s", "abcdefgh");
        return 77;
    }
    TC_RUN(legacy_trunc_survives); TC_RUN(c99_trunc_survives);
    return tc_end();
}
