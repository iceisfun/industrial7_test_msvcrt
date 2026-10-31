/* TC
 * id:    pff_neg_probe_fwprintf_readonly_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf vfwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1 (output error -> negative return)
 * note:  PROBES that fwprintf/vfwprintf with a float to a stream opened "r" returns a negative value, sets ferror, does not run the
 *        invalid-parameter handler and writes nothing to the file.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

#define TMP "tc_pff_neg_probe_fwprintf_readonly_stream.tmp"

static int vfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap); return r; }

TC_CASE(readonly)
{
    FILE *f = fopen(TMP, "w"); int r; long sz;
    CHECK(f != NULL); if (!f) return;
    fclose(f);
    f = fopen(TMP, "r");
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    r = fwprintf(f, L"%f", 1.0);
    CHECK(r < 0);
    CHECK(ferror(f) != 0);
    clearerr(f);
    r = vfw(f, L"%e", 1.0);
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
    f = fopen(TMP, "rb");
    if (f) { fseek(f, 0, SEEK_END); sz = ftell(f); CHECK_EQ_I(sz, 0); fclose(f); }
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(readonly);
    remove(TMP);
    return tc_end();
}
