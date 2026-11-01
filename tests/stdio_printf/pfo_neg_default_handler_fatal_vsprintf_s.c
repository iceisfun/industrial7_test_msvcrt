/* TC
 * id:    pfo_neg_default_handler_fatal_vsprintf_s
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * procs: __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", _set_invalid_parameter_handler (default handler terminates the process)
 * note:  Child-process tests. Exact exit code is probed in pfo_neg_probe_default_handler_exitcode; here only != 0 and != 77 (child survived marker).
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
 *        TEST BUG FIXED (class A, Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "bad_format_fatal c != 77" after the harness fix; zzp_probe2 "vscprintf %y: r=1 errno=0 iph=0"): "%y" is not an invalid
 *        format on ucrt (printed literally, no handler), so it cannot be fatal. bad_format_fatal now uses "%n" (disabled by default -> invalid format -> fatal with
 *        the default handler); the "%y" observation is kept as case unknown_conv_not_fatal (child survives, exit 77). Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vsprintf_s_t)(unsigned long long, char *, size_t, const char *, _locale_t, va_list);
static vsprintf_s_t p_vsprintf_s;
static int vss(char *b, size_t n, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vsprintf_s(0, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}
static int vs(char *b, size_t n, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = __stdio_common_vsprintf(0, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(null_buffer_fatal)  { DWORD c = 0; CHECK(tc_spawn_self("nullbuf", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(null_format_fatal)  { DWORD c = 0; CHECK(tc_spawn_self("nullfmt", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(bad_format_fatal)   { DWORD c = 0; CHECK(tc_spawn_self("badfmt", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(unknown_conv_not_fatal) { DWORD c = 0; CHECK(tc_spawn_self("unknown", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(handler_control)    { DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    p_vsprintf_s = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s");
    if (!p_vsprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        char b[16]; char *volatile nb = NULL; const char *volatile nf = NULL; const char *volatile bf = "%y"; const char *volatile pn = "%n"; int t = 0;
        if (c[0] == 'c') {   /* control */
            int r1, r2;
            if (!tc_iph_install()) return 3;
            tc_iph_count = 0;
            r1 = vss(nb, 16, "abc");
            r2 = vs(b, 16, nf);
            return (r1 == -1 && r2 == -1 && tc_iph_count == 2) ? 77 : 78;
        }
        tc_iph_default();
        if (c[0] == 'n' && c[4] == 'b') vss(nb, 16, "abc");
        else if (c[0] == 'n') vs(b, 16, nf);
        else if (c[0] == 'u') vs(b, 16, bf, 1);      /* "%y": not an error on ucrt (printed literally), child survives */
        else vs(b, 16, pn, &t);                       /* "%n": disabled by default -> invalid format */
        return 77;
    }
    TC_RUN(null_buffer_fatal); TC_RUN(null_format_fatal); TC_RUN(bad_format_fatal); TC_RUN(unknown_conv_not_fatal); TC_RUN(handler_control);
    return tc_end();
}
