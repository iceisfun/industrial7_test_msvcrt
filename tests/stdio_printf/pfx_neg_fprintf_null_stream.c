/* TC
 * id:    pfx_neg_fprintf_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, _fprintf_l ("If stream is a null pointer, the invalid parameter handler is invoked ... returns -1, errno EINVAL")
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_neg_fprintf_null_stream_a_" TC_CRT_DLL ".tmp"

TC_CASE(null_stream)
{
    FILE *volatile nf = NULL; int r;
    if (!tc_iph_install()) TC_SKIP("no _set_invalid_parameter_handler");
    tc_iph_count = 0; errno = 0;
    r = fprintf(nf, "x%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_call_does_not_fire)
{
    FILE *f; int r, before;
    if (!tc_iph_install()) TC_SKIP("no _set_invalid_parameter_handler");
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    before = tc_iph_count;
    r = fprintf(f, "ok");
    CHECK_EQ_I(r, 2);
    CHECK_EQ_I(tc_iph_count, before);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(null_stream); TC_RUN(valid_call_does_not_fire); return tc_end(); }
