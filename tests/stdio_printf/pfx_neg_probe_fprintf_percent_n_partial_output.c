/* TC
 * id:    pfx_neg_probe_fprintf_percent_n_partial_output
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf format specification (%n disabled)
 * note:  Probes what reaches the file when fprintf("ab%ncd") hits the disabled %n: expected exactly "ab" (output produced
 *        before the invalid conversion is kept, nothing after it).
 */
#include "tc.h"
#include <stdio.h>

#define FN "tc_pfx_neg_probe_fprintf_percent_n_partial_output.tmp"

TC_CASE(content)
{
    FILE *f = fopen(FN, "w"); volatile int n = 0; char b[32]; DWORD got = 0; HANDLE h;
    CHECK(f != NULL); if (!f) return;
    fprintf(f, "ab%ncd", (int *)&n);
    fclose(f);
    h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, b, 31, &got, NULL); CloseHandle(h); }
    b[got] = 0;
    CHECK_STR(b, "ab");
    DeleteFileA(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(content); return tc_end(); }
