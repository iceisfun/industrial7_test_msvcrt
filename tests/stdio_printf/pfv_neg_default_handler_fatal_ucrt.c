/* TC
 * id:    pfv_neg_default_handler_fatal_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _set_invalid_parameter_handler: default handler terminates the process
 * note:  UCRT default handler uses __fastfail -> exit code 0xC0000409. Child process cases; child returns 77 if it survives.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile nullfmt = NULL;
static DWORD run(const char *name, int *ran) { DWORD code = 0; *ran = tc_spawn_self(name, &code, 20000); return code; }
TC_CASE(null_format_fatal) { int ran; DWORD c = run("nullfmt", &ran); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
TC_CASE(percent_n_fatal) { int ran; DWORD c = run("pctn", &ran); CHECK(ran); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
TC_CASE(not_survived) { int ran; DWORD c = run("nullfmt", &ran); CHECK(c != 77); c = run("pctn", &ran); CHECK(c != 77); }
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[16]; volatile int x = 0;
        if (c[0] == 'n' && c[1] == 'u') sprintf(b, nullfmt);
        else sprintf(b, "%n", (int *)&x);
        return 77;
    }
    TC_RUN(null_format_fatal); TC_RUN(percent_n_fatal); TC_RUN(not_survived); return tc_end();
}
