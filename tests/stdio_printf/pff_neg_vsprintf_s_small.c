/* TC
 * id:    pff_neg_vsprintf_s_small
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: vsprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs vsprintf_s: too-small or NULL buffer -> invalid parameter handler, -1, buffer[0] = 0 for too-small
 * note:  ucrtbase has vsprintf_s only inline (stdio.h) over __stdio_common_vsprintf_s, so that export is called there.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *vs_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
static vs_t p_vs;
static int call_vs(char *b, size_t n, const char *f, va_list ap) { return p_vs ? p_vs(0, b, n, f, NULL, ap) : -2; }
#define LOAD_VS() ((p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf_s")) != NULL)
#else
typedef int (__cdecl *vs_t)(char *, size_t, const char *, va_list);
static vs_t p_vs;
static int call_vs(char *b, size_t n, const char *f, va_list ap) { return p_vs ? p_vs(b, n, f, ap) : -2; }
#define LOAD_VS() ((p_vs = (vs_t)tc_crt_proc("vsprintf_s")) != NULL)
#endif

static int w(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = call_vs(b, n, f, ap); va_end(ap); return r; }

TC_CASE(too_small)
{
    char b[32]; volatile size_t n = 4; int r, i;
    for (i = 0; i < 32; i++) b[i] = 0x5A;
    tc_iph_count = 0;
    r = w(b, n, "%e", 1.5);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(b[4], 0x5A);
}
TC_CASE(null_buffer)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = w(b, 16, "%e", 1.5);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(roomy_succeeds)
{
    char b[32]; volatile size_t n = 32; int r;
    tc_iph_count = 0;
    r = w(b, n, "%.1f", 1.5);
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "1.5");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!LOAD_VS()) { tc_write("TC FAIL " TC_TEST_ID "::__vsprintf_s not exported\n"); return 2; }
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(null_buffer); TC_RUN(roomy_succeeds); return tc_end();
}
