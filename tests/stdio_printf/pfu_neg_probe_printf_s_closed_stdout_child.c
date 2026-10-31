/* TC
 * id:    pfu_neg_probe_printf_s_closed_stdout_child
 * area:  stdio_printf
 * kind:  negative
 * funcs: printf_s wprintf_s setvbuf _close
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf_s, _close; errno EBADF
 * note:  PROBE: in a child with an invalid-parameter handler installed, stdout made unbuffered then fd 1 closed, printf_s("x")
 *        (and wprintf_s) returns -1 with errno EBADF (the flush/write to a closed fd fails). The child exits 77 if satisfied,
 *        20 if the return was not -1, 21 if errno was not EBADF. A control child proves child plumbing (77).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(narrow) { DWORD c = 0; CHECK(tc_spawn_self("narrow", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(wide)   { DWORD c = 0; CHECK(tc_spawn_self("wide", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(control){ DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int r, e;
        if (c[0] == 'c') return 77;
        tc_iph_install();
        setvbuf(stdout, NULL, _IONBF, 0);
        _close(1);
        errno = 0;
        if (c[0] == 'n') r = printf_s("x"); else r = wprintf_s(L"x");
        e = errno;
        if (r != -1) return 20;
        if (e != EBADF) return 21;
        return 77;
    }
    TC_RUN(narrow); TC_RUN(wide); TC_RUN(control);
    return tc_end();
}
