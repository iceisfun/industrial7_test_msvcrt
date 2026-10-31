/* TC
 * id:    pfx_fprintf_null_string_arg
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs "Format specification syntax" (%s); widely observed CRT behaviour: NULL string argument prints "(null)"
 * note:  Not guaranteed by the C standard (undefined) and not spelled out in the docs; observed on all MS CRTs.
 *        The %.3s-with-NULL case lives in pfx_fprintf_null_string_prec_probe.
 */
#include "tc.h"
#include <stdio.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_fprintf_null_string_arg_a_" TC_CRT_DLL ".tmp"

TC_CASE(percent_s_null)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; const char *volatile p = NULL;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%s", p);
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "(null)", 6);
    remove(FN);
}
TC_CASE(percent_ls_null)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; const wchar_t *volatile p = NULL;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%ls", p);
    fclose(f);
    CHECK_EQ_I(r, 6);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 6); CHECK_MEM(b, "(null)", 6);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(percent_s_null); TC_RUN(percent_ls_null); return tc_end(); }
