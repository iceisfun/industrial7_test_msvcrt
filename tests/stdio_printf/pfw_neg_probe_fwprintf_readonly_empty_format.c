/* TC
 * id:    pfw_neg_probe_fwprintf_readonly_empty_format
 * area:  stdio_printf
 * funcs: fwprintf fopen fclose remove
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: fwprintf to a read-only stream with output of zero characters (L"" and %s with L""). Windows may return 0 (nothing to write) or -1/EBADF; the test accepts either and just records no crash. Control: one character returns -1.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#define FN "tc_pfw_neg_probe_fwprintf_readonly_empty_format_" TC_STR(TC_CRT_VER) ".tmp"
static const wchar_t *volatile fe = L"";
static const wchar_t *volatile fs = L"%s";
static const wchar_t *volatile fx = L"x";

TC_CASE(empty)
{
    FILE *f = fopen(FN, "wb"), *fp; int n;
    CHECK(f != NULL); if (!f) return; fputc('Q', f); fclose(f);
    fp = fopen(FN, "r"); CHECK(fp != NULL); if (!fp) { remove(FN); return; }
    n = fwprintf(fp, fe); CHECK(n == 0 || n == -1);
    n = fwprintf(fp, fs, fe); CHECK(n == 0 || n == -1);
    CHECK_EQ_I(fwprintf(fp, fx), -1);
    fclose(fp); remove(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(empty); return tc_end(); }
