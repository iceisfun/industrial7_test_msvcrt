/* TC
 * id:    pfr_neg_default_handler_sprintf_s_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation; sprintf_s; _set_invalid_parameter_handler (default handler terminates)
 * note:  Child-process test. Only "dies, code != 77, != 0" asserted; exact code is probed elsewhere.
 *        The control child (handler installed) proves the failing calls return -1 and invoke the handler.
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
 *        Second defect: the TC_UCRT branch was never compiled (tools/tc.py never defines it), so SPF_S called the NULL p_sprintf_s (exit 0xC0000005 in
 *        the control child, and the two fatal children "died" of an access violation, passing vacuously). The branch is now selected locally with
 *        TC_CRT_VER >= 140 (tools/tc.py is unchanged; defining TC_UCRT globally would flip 59 other tests).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *sprintf_s_t)(char *, size_t, const char *, ...);
static sprintf_s_t p_sprintf_s;
#if TC_CRT_VER >= 140
/* sprintf_s is an inline wrapper over __stdio_common_vsprintf_s in the UCRT headers; call that export directly so the
 * test provably runs ucrtbase code (mingw otherwise links its own static sprintf_s). */
typedef int (__cdecl *vsps_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsps_t p_vsps;
static int ucrt_sprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsps(0, b, n, f, NULL, ap); va_end(ap); return r; }
#define SPF_S(b, n, f) ucrt_sprintf_s(b, n, f)
#else
#define SPF_S(b, n, f) p_sprintf_s(b, n, f)
#endif

TC_CASE(small_buffer_fatal) { DWORD c = 0; CHECK(tc_spawn_self("small", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(null_buffer_fatal)  { DWORD c = 0; CHECK(tc_spawn_self("nullbuf", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(control)            { DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    p_sprintf_s = (sprintf_s_t)tc_crt_proc("sprintf_s");
#if TC_CRT_VER >= 140
    p_vsps = (vsps_t)tc_crt_proc("__stdio_common_vsprintf_s");
    if (!p_vsps) { tc_write("TC FAIL " TC_TEST_ID "::__proc __stdio_common_vsprintf_s missing\n"); return 1; }
#endif
#if TC_CRT_VER < 140
    if (!p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc sprintf_s missing\n"); return 1; }
#endif
    if ((c = tc_child_name()) != NULL) {
        char b[8]; char *volatile nb = NULL; int r1, r2;
        if (c[0] == 'c') {
            if (!tc_iph_install()) return 3;
            tc_iph_count = 0;
            r1 = SPF_S(b, 2, "abcdef");
            r2 = SPF_S(nb, 8, "abc");
            return (r1 == -1 && r2 == -1 && tc_iph_count == 2) ? 77 : (100 + (r1 == -1) + 2 * (r2 == -1) + 4 * tc_iph_count);
        }
        tc_iph_default();
        if (c[0] == 's') SPF_S(b, 2, "abcdef");
        else SPF_S(nb, 8, "abc");
        return 77;
    }
    TC_RUN(small_buffer_fatal); TC_RUN(null_buffer_fatal); TC_RUN(control);
    return tc_end();
}
