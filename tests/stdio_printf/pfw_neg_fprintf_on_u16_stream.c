/* TC
 * id:    pfw_neg_fprintf_on_u16_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf _wfopen
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen, _wfopen: Unicode (ccs=UTF-16LE) streams require wide-character I/O functions; narrow functions are invalid
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN L"tc_pfw_neg_fprintf_on_u16_stream_a.tmp"

TC_CASE(narrow_on_u16)
{
    FILE *f = _wfopen(FN, L"w, ccs=UTF-16LE"); int r; unsigned char buf[64]; size_t n, i; int found = 0;
    if (!f) TC_SKIP("cannot create UTF-16 stream");
    tc_iph_count = 0;
    r = fprintf(f, "abc%d", 7);
    CHECK(r < 0);
    CHECK(tc_iph_count >= 1);
    fclose(f);
    f = _wfopen(FN, L"rb");
    if (!f) TC_SKIP("cannot reopen");
    n = fread(buf, 1, sizeof buf, f); fclose(f);
    for (i = 0; i + 2 < n; i++) if (buf[i] == 'a' && buf[i+1] == 'b' && buf[i+2] == 'c') found = 1;
    CHECK(!found);
    _wunlink(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph API\n"); return 1; }
    TC_RUN(narrow_on_u16);
    return tc_end();
}
