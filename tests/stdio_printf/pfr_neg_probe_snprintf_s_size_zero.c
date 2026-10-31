/* TC
 * id:    pfr_neg_probe_snprintf_s_size_zero
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnprintf_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s (invalid size parameters -> EINVAL)
 * note:  PROBE: non-NULL buffer with sizeOfBuffer == 0 and count 10. Expected most likely: returns -1, errno EINVAL, handler called once, buffer untouched. Current MS docs (_snprintf_s behavior table, buffer != NULL && sizeOfBuffer == 0) say EINVAL + handler, but that table is for the UCRT docs; msvcr80-120 may report ERANGE, so kept as probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));


/* ucrtbase: mingw inlines the *_s printf family onto __stdio_common_vsnprintf_s; older CRTs: call the DLL export. */
static int w_snprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f);
#if TC_CRT_VER >= 140
    r = _vsnprintf_s(b, n, c, f, ap);
#else
    r = tc_p__vsnprintf_s(b, n, c, f, ap);
#endif
    va_end(ap); return r;
}
static int w_vsnprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap)
{
#if TC_CRT_VER >= 140
    return _vsnprintf_s(b, n, c, f, ap);
#else
    return tc_p__vsnprintf_s(b, n, c, f, ap);
#endif
}

TC_CASE(size_zero)
{
    char b[8]; volatile size_t n = 0, c = 10; int r, i;
    for (i = 0; i < 8; i++) b[i] = 0x5a;
    tc_iph_count = 0; errno = 0;
    r = w_snprintf_s(b, n, c, "%s", "abc");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I((unsigned char)b[0], 0x5a);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_vsnprintf_s);
#if TC_CRT_VER < 140
    if (!tc_p__vsnprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(size_zero);
    return tc_end();
}
