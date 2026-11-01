/* TC
 * id:    cv_neg_itoa_s_fatal_default
 * area:  conv
 * funcs: _itoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default invalid-parameter handler terminates the process
 * note:  Exit code asserted only != 77 and != 0 (exact status differs by CRT). Fatal paths run in child processes.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(null_buf) { DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000); CHECK(ran); CHECK(code != 77); CHECK(code != 0); }
TC_CASE(tiny_buf) { DWORD code = 0; int ran = tc_spawn_self("tiny", &code, 20000); CHECK(ran); CHECK(code != 77); CHECK(code != 0); }
TC_CASE(control_valid) { DWORD code = 0; int ran = tc_spawn_self("valid", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }

int main(void)
{
    const char *c; char b[16]; char *volatile nb = NULL; volatile size_t n3 = 3, n16 = 16, n0 = 0;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        if (c[0] == 'n') _itoa_s(1, nb, n0, 10);
        else if (c[0] == 't') _itoa_s(12345, b, n3, 10);
        else { if (_itoa_s(1, b, n16, 10) != 0) return 5; }
        return 77;
    }
    TC_RUN(null_buf); TC_RUN(tiny_buf); TC_RUN(control_valid);
    return tc_end();
}
