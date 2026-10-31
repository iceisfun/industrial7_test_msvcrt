/* TC
 * id:    pfw_neg_wprintf_closed_stdout_child
 * area:  stdio_printf
 * kind:  negative
 * funcs: wprintf
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.29.2.11 (output error -> negative); MS docs _write (closed fd -> EBADF)
 * note:  Child process closes fd 1, makes stdout unbuffered so the write happens inside wprintf, and installs an
 *        invalid-parameter handler (writing to a closed fd is also an invalid parameter). PROBED: wprintf returns -1, errno == EBADF, no crash
 *        (docs give EBADF for _write on a closed fd; propagation through wprintf is inferred).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(child)
{
    DWORD code = 99; int ran = tc_spawn_self("closed", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL)
    {
        int r;
        tc_iph_install();
        _close(1);
        setvbuf(stdout, NULL, _IONBF, 0);
        errno = 0;
        r = wprintf(L"x%d\n", 1);
        if (r != -1) return 10;
        if (errno != EBADF) return 11;
        return 0;
    }
    TC_RUN(child);
    return tc_end();
}
