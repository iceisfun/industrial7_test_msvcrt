/* TC
 * id:    pff_neg_fatal_exitcode_ucrt_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation (exact status undocumented)
 * note:  probes: ucrt invalid-parameter abort uses __fastfail -> 0xC0000409; a WER/AeDebug configuration may change this
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
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

TC_CASE(exit_code)
{
    DWORD code = 0; int ran = tc_spawn_self("fatal", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin(); CHECK(LOAD_SPRINTF_S());
    if (tc_child_name()) { char d[2]; tc_iph_default(); volatile double v = 1.5; xsprintf_s(d, 2, "%f", v); return 77; }
    TC_RUN(exit_code);
    return tc_end();
}
