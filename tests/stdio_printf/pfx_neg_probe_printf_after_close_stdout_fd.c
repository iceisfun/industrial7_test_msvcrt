/* TC
 * id:    pfx_neg_probe_printf_after_close_stdout_fd
 * area:  stdio_printf
 * kind:  negative
 * funcs: printf wprintf setvbuf _close
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _close, setvbuf, printf (negative on error)
 * note:  PROBES that after stdout is made unbuffered and its fd 1 is closed with _close(1), printf/wprintf return a negative value
 *        and errno == EBADF. Run in child processes with a returning invalid-parameter handler installed (closed-fd writes invoke it). Exit code = 0x40 | 1 (r<0) | 2 (errno==EBADF).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <io.h>
#include <errno.h>

static const char *volatile g_x = "x";
static const wchar_t *volatile g_wx = L"x";

static int child_main(const char *n)
{
    int r, e, code = 0x40;
    tc_iph_install();   /* _write/_isatty on a closed fd call the invalid-parameter handler; the default one is fatal */
    setvbuf(stdout, NULL, _IONBF, 0);
    _close(1);
    errno = 0;
    if (n[0] == 'w') r = wprintf(g_wx); else r = printf(g_x);
    e = errno;
    if (r < 0) code |= 1;
    if (e == EBADF) code |= 2;
    return code;
}
TC_CASE(narrow)
{
    DWORD code = 0; int ran = tc_spawn_self("narrow", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0x43);
}
TC_CASE(wide)
{
    DWORD code = 0; int ran = tc_spawn_self("wide", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0x43);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(narrow); TC_RUN(wide);
    return tc_end();
}
