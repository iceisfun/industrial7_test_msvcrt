/* TC
 * id:    pff_neg_sprintf_s_default_handler_fatal
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default invalid parameter handler terminates the process
 * note:  child process only. msvcrt.dll is covered separately by pff_neg_sprintf_s_default_handler_fatal_msvcrt_probe.
 *        Win10 22H2 fix: the fatal child first calls tc_iph_default() (mingw crtexe installs a no-op invalid-parameter handler, so the default path was never reached).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsprintf_s_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
static vsprintf_s_t p_common;
static int xsprintf_s(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, f, NULL, ap); va_end(ap); return r; }
#define LOAD_SPRINTF_S() ((p_common = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s")) != NULL)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define xsprintf_s tc_p_sprintf_s
#define LOAD_SPRINTF_S() (TC_LOAD_PROC(sprintf_s) != NULL)
#endif

static int child_main(const char *name)
{
    char d[2]; volatile double v = 1.5;
    if (name[0] == 'c') {   /* control: handler installed */
        int n;
        if (!tc_iph_install()) return 1;
        n = xsprintf_s(d, 2, "%f", v);
        return n == -1 ? 77 : 2;
    }
    tc_iph_default();       /* mingw startup installs a no-op handler; restore the CRT default */
    xsprintf_s(d, 2, "%f", v);
    return 77;              /* reached only if the CRT did not terminate */
}
TC_CASE(fatal_child)
{
    DWORD code = 0; int ran = tc_spawn_self("fatal", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(control_child)
{
    DWORD code = 0; int ran = tc_spawn_self("control", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin(); CHECK(LOAD_SPRINTF_S());
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(fatal_child); TC_RUN(control_child);
    return tc_end();
}
