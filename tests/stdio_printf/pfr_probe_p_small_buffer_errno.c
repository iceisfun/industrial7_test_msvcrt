/* TC
 * id:    pfr_probe_p_small_buffer_errno
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_p, __stdio_common_vsprintf_p
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _sprintf_p (by analogy with sprintf_s)
 * note:  PROBING, each in its own case, what _sprintf_p(b,3,"abcdef") does on a too-small buffer: (a) returns -1 and
 *        invokes the invalid-parameter handler, (b) sets errno ERANGE, (c) writes b[0]=0 (all as sprintf_s does).
 *        OBSERVED on ucrtbase (Win10 22H2 19045 reference integration full run triage-r1): the export __stdio_common_vsprintf_p(options 0, b, 3, "abcdef")
 *        returns -2, does NOT call the handler, leaves errno 0 and keeps the truncated output (b[0]=='a'). Class A: the UCRT branch
 *        asserts those values (the baseline build skipped every case: ucrtbase has no _sprintf_p export and TC_UCRT was undefined).
 *        Older CRTs keep the original guesses. Cases renamed ret_and_handler/errno_value/buffer_state.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <stdarg.h>

typedef int (__cdecl *sprintf_p_t)(char *, size_t, const char *, ...);
typedef int (__cdecl *scprintf_p_t)(const char *, ...);
static sprintf_p_t p_sprintf_p;
static scprintf_p_t p_scprintf_p;
#ifdef TC_UCRT
/* ucrtbase does not export _sprintf_p/_scprintf_p (inline wrappers over __stdio_common_vsprintf_p): call the export. */
typedef int (__cdecl *vsp_p_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsp_p_t p_vsp_p;
static int ucrt_sprintf_p(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int ucrt_scprintf_p(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp_p(2, NULL, 0, f, NULL, ap); va_end(ap); return r; }
#endif
static void p_setup(void)
{
#ifdef TC_UCRT
    p_vsp_p = (vsp_p_t)tc_crt_proc("__stdio_common_vsprintf_p");
    if (p_vsp_p) { p_sprintf_p = ucrt_sprintf_p; p_scprintf_p = ucrt_scprintf_p; }
#else
    p_sprintf_p = (sprintf_p_t)tc_crt_proc("_sprintf_p");
    p_scprintf_p = (scprintf_p_t)tc_crt_proc("_scprintf_p");
#endif
}

TC_CASE(ret_and_handler)
{
    char b[8]; int r;
    if (!p_sprintf_p) TC_SKIP("_sprintf_p not available");
    b[0] = 'Q'; tc_iph_count = 0;
    r = p_sprintf_p(b, 3, "abcdef");
#ifdef TC_UCRT
    CHECK_EQ_I(r, -2); CHECK_EQ_I(tc_iph_count, 0);   /* observed on Win10 22H2 */
#else
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1);
#endif
}
TC_CASE(errno_value)
{
    char b[8]; int r;
    if (!p_sprintf_p) TC_SKIP("_sprintf_p not available");
    b[0] = 'Q'; errno = 0;
    r = p_sprintf_p(b, 3, "abcdef");
#ifdef TC_UCRT
    CHECK_EQ_I(r, -2); CHECK_EQ_I(errno, 0);   /* observed on Win10 22H2 */
#else
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE);
#endif
}
TC_CASE(buffer_state)
{
    char b[8]; int r;
    if (!p_sprintf_p) TC_SKIP("_sprintf_p not available");
    memset(b, 'Q', sizeof b);
    r = p_sprintf_p(b, 3, "abcdef");
#ifdef TC_UCRT
    CHECK_EQ_I(r, -2); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 'b'); CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[3], 'Q');   /* observed on Win10 22H2 */
#else
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0);
#endif
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_setup();
    TC_RUN(ret_and_handler); TC_RUN(errno_value); TC_RUN(buffer_state);
    return tc_end();
}
