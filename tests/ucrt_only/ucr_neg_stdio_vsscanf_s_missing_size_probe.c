/* TC
 * id:    ucr_neg_stdio_vsscanf_s_missing_size_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs scanf_s (buffer size argument required after %s/%c/%[); Parameter validation
 * note:  (size argument is passed as size_t, the rsize_t the callee reads from the va_list)
 *        PROBES secure-CRT (option bit 1) behaviour when the size argument is 0 (%s, %c) or too small for the scanset
 *        (size 1, %[a-z] needs room for NUL): return 0, no write past the given size, buf[0] cleared for %s/%[ (from memory).
 *        errno may be unset or ERANGE; both accepted. Invalid-parameter handler is installed and counted (not asserted).
 */
#include "tc.h"
#include <stdarg.h>
#include <string.h>
#include <errno.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;

static int xs(const char *in, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vss(1ULL, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(s_size0)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b); errno = 0;
    r = xs("abc", "%s", b, (size_t)0);
    CHECK_EQ_I(r, 0);
    CHECK(b[0] == 'z' || b[0] == 0);
    CHECK(b[1] == 'z');
    CHECK(errno == 0 || errno == ERANGE);
}
TC_CASE(c_size0)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b); errno = 0;
    r = xs("abc", "%c", b, (size_t)0);
    CHECK_EQ_I(r, 0);
    CHECK(b[0] == 'z' || b[0] == 0);
    CHECK(b[1] == 'z');
    CHECK(errno == 0 || errno == ERANGE);
}
TC_CASE(scanset_size1)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b);
    r = xs("abc", "%[a-z]", b, (size_t)1);
    CHECK_EQ_I(r, 0);
    CHECK(b[0] == 0);
    CHECK(b[1] == 'z');
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(s_size0); TC_RUN(c_size0); TC_RUN(scanset_size1);
    return tc_end();
}
