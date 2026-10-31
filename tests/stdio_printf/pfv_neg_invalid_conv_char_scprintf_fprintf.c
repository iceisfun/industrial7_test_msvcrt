/* TC
 * id:    pfv_neg_invalid_conv_char_scprintf_fprintf
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf fprintf _snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", format specification syntax: unknown conversion -> invalid parameter handler, -1, EINVAL
 * note:  The same invalid-format rule applies to the length-counting, stream and counted-buffer entry points, not only sprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_y = "%y";

TC_CASE(scprintf_y) { int r; errno = 0; tc_iph_count = 0; r = _scprintf(f_y); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(snprintf_y) { char b[8]; int r; errno = 0; tc_iph_count = 0; r = _snprintf(b, 8, f_y); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fprintf_y)
{
    static const char name[] = "tc_pfv_neg_invalid_conv_char_scprintf_fprintf_a.tmp";
    FILE *f = fopen(name, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0; r = fprintf(f, f_y);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(name);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(scprintf_y); TC_RUN(snprintf_y); TC_RUN(fprintf_y);
    return tc_end();
}
