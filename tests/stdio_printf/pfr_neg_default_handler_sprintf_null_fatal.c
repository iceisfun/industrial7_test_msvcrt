/* TC
 * id:    pfr_neg_default_handler_sprintf_null_fatal
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf, _scprintf
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf, _scprintf (NULL arguments invoke the invalid parameter handler); Parameter validation
 * note:  TC_UCRT is never defined by tools/tc.py, so the old #ifdef selected the GetProcAddress("sprintf") path, which is NULL on ucrtbase: every child died with 0xC0000005
 *        calling a NULL pointer (the "fatal" cases passed by accident). Now selected with TC_CRT_VER >= 140. Children restore the CRT default handler (tc_iph_default).
 *        Child-process test; control child has a handler installed and expects -1 / 2 handler calls.
 *        sprintf is exported by msvcr80..120 but is an inline wrapper in the ucrt headers, so on ucrtbase the test calls
 *        the __stdio_common_vsprintf export directly with the same arguments the inline wrapper would pass.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *sprintf_t)(char *, const char *, ...);
typedef int (__cdecl *scprintf_t)(const char *, ...);
static sprintf_t p_sprintf;
static scprintf_t p_scprintf;
#if TC_CRT_VER >= 140
/* sprintf/_scprintf are inline wrappers over __stdio_common_vsprintf in the UCRT headers (sprintf passes count
 * (size_t)-1; _scprintf passes options 2 = C99 snprintf behaviour, NULL buffer, count 0). Call the export directly so the
 * test provably runs ucrtbase code. */
typedef int (__cdecl *vsp_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsp_t p_vsp;
static int ucrt_sprintf(char *b, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp(0, b, (size_t)-1, f, NULL, ap); va_end(ap); return r; }
static int ucrt_scprintf(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp(2, NULL, 0, f, NULL, ap); va_end(ap); return r; }
#define SCPF(f) ucrt_scprintf(f)
#define do_sprintf(b, f) ucrt_sprintf(b, f)
#else
#define SCPF(f) p_scprintf(f)
#define do_sprintf(b, f) p_sprintf(b, f)
#endif

TC_CASE(sprintf_null_fatal)   { DWORD c = 0; CHECK(tc_spawn_self("spnull", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(scprintf_null_fatal)  { DWORD c = 0; CHECK(tc_spawn_self("scnull", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }
TC_CASE(control)              { DWORD c = 0; CHECK(tc_spawn_self("control", &c, 20000)); CHECK_EQ_U(c, 77); }

int main(void)
{
    const char *c;
    tc_begin();
    p_sprintf = (sprintf_t)tc_crt_proc("sprintf");
    p_scprintf = (scprintf_t)tc_crt_proc("_scprintf");
#if TC_CRT_VER >= 140
    p_vsp = (vsp_t)tc_crt_proc("__stdio_common_vsprintf");
    if (!p_vsp) { tc_write("TC FAIL " TC_TEST_ID "::__proc __stdio_common_vsprintf missing\n"); return 1; }
#endif
#if TC_CRT_VER < 140
    if (!p_sprintf) { tc_write("TC FAIL " TC_TEST_ID "::__proc sprintf missing\n"); return 1; }
#endif
    if ((c = tc_child_name()) != NULL) {
        char *volatile nb = NULL; const char *volatile nf = NULL; int r1, r2;
        if (c[0] == 'c') {
            if (!tc_iph_install()) return 3;
            tc_iph_count = 0;
            r1 = do_sprintf(nb, "x");
            r2 = SCPF(nf);
            return (r1 == -1 && r2 == -1 && tc_iph_count == 2) ? 77 : (100 + (r1 == -1) + 2 * (r2 == -1) + 4 * tc_iph_count);
        }
        tc_iph_default();
        if (c[0] == 's' && c[1] == 'p') do_sprintf(nb, "x");
        else SCPF(nf);
        return 77;
    }
    TC_RUN(sprintf_null_fatal); TC_RUN(scprintf_null_fatal); TC_RUN(control);
    return tc_end();
}
