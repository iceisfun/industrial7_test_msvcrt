/* TC
 * id:    pfx_fprintf_neg_null_format
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf; Parameter validation: format NULL -> invalid parameter handler, errno EINVAL, returns -1
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static DWORD slurp(const char *n, unsigned char *b, DWORD cap)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return (DWORD)-1;
    if (!ReadFile(h, b, cap, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return got;
}

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_neg_null_format_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(null_format)
{
    unsigned char b[4]; int r; volatile const char *fmt = NULL;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    if (!tc_iph_install()) { fclose(f); remove(FN); TC_SKIP("no handler API"); }
    tc_iph_count = 0; errno = 0;
    r = fprintf(f, (const char *)fmt);
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    fclose(f);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 0);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(null_format);
    return tc_end();
}
