/* TC
 * id:    scn_common_vsscanf_secure_flag
 * area:  scanf
 * funcs: __stdio_common_vsscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sscanf_s / scanf_s (size argument follows the buffer for %s %c %[); stdio.h inline sscanf_s passes _CRT_INTERNAL_SCANF_SECURECRT (1)
 * note:  options = 1 reproduces what the inline sscanf_s does. Without the flag the size argument must NOT be consumed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cs(unsigned long long opt, const char *in, const char *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
    r = __stdio_common_vsscanf(opt, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap); return r;
}
static const char *volatile vin;

TC_CASE(s_size)
{
    char b[8]; int i, n, v = 0;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    vin = "abc 5";
    n = cs(1, (const char *)vin, "%s %d", b, (unsigned)8, &v);
    CHECK_EQ_I(n, 2); CHECK_STR(b, "abc"); CHECK_EQ_I(v, 5);
}
TC_CASE(c_size)
{
    char b[4]; int n, v = 0;
    b[0] = b[1] = b[2] = b[3] = 'Z';
    vin = "xy 6";
    n = cs(1, (const char *)vin, "%2c %d", b, (unsigned)2, &v);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(b[0], 'x'); CHECK_EQ_I(b[1], 'y'); CHECK_EQ_I(b[2], 'Z'); CHECK_EQ_I(v, 6);
}
TC_CASE(scanset_size)
{
    char b[8]; int n;
    b[0] = 'Z';
    vin = "abc123";
    n = cs(1, (const char *)vin, "%[a-c]", b, (unsigned)8);
    CHECK_EQ_I(n, 1); CHECK_STR(b, "abc");
}
TC_CASE(without_flag_no_size)
{
    char b[8]; int n, v = 0;
    vin = "abc 5";
    n = cs(0, (const char *)vin, "%s %d", b, &v);
    CHECK_EQ_I(n, 2); CHECK_STR(b, "abc"); CHECK_EQ_I(v, 5);
}
int main(void)
{
    tc_begin();
    TC_RUN(s_size); TC_RUN(c_size); TC_RUN(scanset_size); TC_RUN(without_flag_no_size);
    return tc_end();
}
