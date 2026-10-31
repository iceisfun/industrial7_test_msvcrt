/* TC
 * id:    pfx_fprintf_neg_readonly_stream
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ferror
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf: negative value on output error; C11 7.21.6.1p3
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
#define FN "tc_pfx_fprintf_neg_readonly_stream_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(write_to_read_stream)
{
    unsigned char b[8]; int r; FILE *f;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fprintf(f, "orig"); fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    (void)tc_iph_install();   /* keep a possible invalid-parameter call non-fatal; not asserted */
    r = fprintf(f, "more");
    CHECK(r < 0);
    CHECK(ferror(f) != 0);
    fclose(f);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 4);
    CHECK_MEM(b, "orig", 4);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(write_to_read_stream);
    return tc_end();
}
