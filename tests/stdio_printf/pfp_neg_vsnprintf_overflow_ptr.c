/* TC
 * id:    pfp_neg_vsnprintf_overflow_ptr
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf, _vsnprintf_l; printf %p (x64: 16 uppercase hex digits, zero padded)
 * note:  ucrtbase has no vsnprintf/_vsnprintf exports (inline wrappers over __stdio_common_vsprintf), so the wrappers'
 *        option words are used: _vsnprintf = LEGACY_VSPRINTF_NULL_TERMINATION (1), vsnprintf = STANDARD_SNPRINTF_BEHAVIOR (2).
 *        "%p" of 0x1234 is 16 chars on x64. _vsnprintf returns -1 when output does not fit and does not NUL-terminate
 *        when exactly count chars are written; C99 vsnprintf returns the would-be length and always terminates.
 */
#include "tc.h"
#include <stdarg.h>
#include <stddef.h>

typedef int (__cdecl *vsn_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsn_t p_vsn;

/* _vsnprintf: result < 0 maps to -1 */
static int call_old(char *b, size_t n, const char *f, ...)
{ int r; va_list ap; va_start(ap, f); r = p_vsn(1ULL, b, n, f, NULL, ap); va_end(ap); return r < 0 ? -1 : r; }
/* vsnprintf (C99) */
static int call_c99(char *b, size_t n, const char *f, ...)
{ int r; va_list ap; va_start(ap, f); r = p_vsn(2ULL, b, n, f, NULL, ap); va_end(ap); return r < 0 ? -1 : r; }

#define PTR ((void *)(ULONG_PTR)0x1234)

TC_CASE(old_too_small)
{
    unsigned char b[32]; int i, r; void *volatile p = PTR; const char *volatile f = "%p";
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 32; i++) b[i] = 0xCC;
    r = call_old((char *)b, 8, f, p);
    CHECK_EQ_I(r, -1);
    for (i = 8; i < 32; i++) CHECK_EQ_I(b[i], 0xCC);
}
TC_CASE(old_exact_fit)
{
    unsigned char b[32]; int i, r; void *volatile p = PTR; const char *volatile f = "%p";
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 32; i++) b[i] = 0xCC;
    r = call_old((char *)b, 16, f, p);
    CHECK_EQ_I(r, 16);
    CHECK_MEM(b, "0000000000001234", 16);
    CHECK_EQ_I(b[16], 0xCC);   /* no terminator written */
}
TC_CASE(c99_truncates)
{
    char b[32]; int i, r; void *volatile p = PTR; const char *volatile f = "%p";
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 32; i++) b[i] = (char)0xCC;
    r = call_c99(b, 8, f, p);
    CHECK_EQ_I(r, 16);
    CHECK_MEM(b, "0000000", 7);
    CHECK_EQ_I(b[7], 0);
    CHECK_EQ_I((unsigned char)b[8], 0xCC);
}
TC_CASE(c99_null_zero)
{
    void *volatile p = PTR; const char *volatile f = "%p";
    if (!TC_X64) TC_SKIP("x64 only");
    CHECK_EQ_I(call_c99(NULL, 0, f, p), 16);
}

int main(void)
{
    tc_begin();
    p_vsn = (vsn_t)tc_crt_proc("__stdio_common_vsprintf");
    if (!p_vsn) { tc_write("TC FAIL " TC_TEST_ID "::__stdio_common_vsprintf missing\n"); return 1; }
    TC_RUN(old_too_small); TC_RUN(old_exact_fit); TC_RUN(c99_truncates); TC_RUN(c99_null_zero);
    return tc_end();
}
