/* TC
 * id:    nfx_neg_iph_default_exact_status_ucrt_probe
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler memcpy_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s, "Parameter validation"
 * note:  PROBE: exit status of the default invalid-parameter path via memcpy_s(NULL,10,src,5) with no handler.
 *        Expected 0xC0000409 (fast fail) in release ucrtbase. (tc_begin already sets SEM_NOGPFAULTERRORBOX in
 *        both children, so 'plain' and 'nogpf' differ only by a redundant re-set.)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

TC_CASE(plain) { DWORD code = 0; CHECK(tc_spawn_self("plain", &code, 20000)); CHECK_EQ_U(code, TC_STATUS_FASTFAIL); }
TC_CASE(nogpf) { DWORD code = 0; CHECK(tc_spawn_self("nogpf", &code, 20000)); CHECK_EQ_U(code, TC_STATUS_FASTFAIL); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char src[8] = "abcdefg"; void *volatile d = NULL; volatile size_t n = 5;
        if (c[0] == 'n') SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        _set_invalid_parameter_handler(NULL);
        memcpy_s((void *)d, 10, src, n);
        return 77;
    }
    TC_RUN(plain); TC_RUN(nogpf);
    return tc_end();
}
