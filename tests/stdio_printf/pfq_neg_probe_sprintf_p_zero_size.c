/* TC
 * id:    pfq_neg_probe_sprintf_p_zero_size
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p _swprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs sprintf_p (sizeOfBuffer); behaviour for size 0 is not spelled out
 * note:  PROBES: _sprintf_p / _swprintf_p with a valid buffer and sizeOfBuffer == 0 return -1 and write nothing to the buffer
 *        (0xAA / 0xAAAA pre-fill stays). Whether the invalid parameter handler runs and errno==EINVAL is only RECORDED
 *        as a "TC NOTE" line. NULL buffer with size 0 is only recorded as well.
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

static void note(const char *what, int r, int e)
{
    char n[24], c[24], x[24];
    tc_i64(r, n); tc_i64(tc_iph_count, c); tc_i64(e, x);
    tc_write("TC NOTE " TC_TEST_ID "::"); tc_write(what); tc_write(" ret="); tc_write(n);
    tc_write(" iph="); tc_write(c); tc_write(" errno="); tc_write(x); tc_write("\n");
}
TC_CASE(narrow_zero)
{
    unsigned char b[4]; volatile size_t n = 0; int r;
    memset(b, 0xAA, sizeof b); tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p((char *)b, n, "%1$d", 1);
    note("narrow_zero", r, errno);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[0], 0xAA);
}
TC_CASE(wide_zero)
{
    wchar_t b[4]; volatile size_t n = 0; int r; int i;
    for (i = 0; i < 4; i++) b[i] = 0xAAAA;
    tc_iph_count = 0; errno = 0;
    r = pq_swprintf_p(b, n, L"%1$d", 1);
    note("wide_zero", r, errno);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[0], 0xAAAA);
}
TC_CASE(null_zero_record_only)
{
    char * volatile b = NULL; volatile size_t n = 0; int r;
    tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p(b, n, "%1$d", 1);
    note("null_zero", r, errno);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(narrow_zero); TC_RUN(wide_zero); TC_RUN(null_zero_record_only);
    return tc_end();
}
