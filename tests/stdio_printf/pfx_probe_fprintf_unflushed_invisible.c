/* TC
 * id:    pfx_probe_fprintf_unflushed_invisible
 * area:  stdio_printf
 * funcs: fprintf fopen setvbuf fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.3 (fully buffered: written when buffer fills/flush)
 * note:  PROBE: with a 4096-byte fully-buffered stream, 10 bytes written by fprintf are NOT yet visible to an external handle (size 0) before fflush/fclose. Nothing else asserted.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#define F "tc_pfx_probe_fprintf_unflushed_invisible_" TCV ".tmp"

static long ext_size(void)
{
    LARGE_INTEGER sz; HANDLE h = CreateFileA(F, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!GetFileSizeEx(h, &sz)) sz.QuadPart = -1;
    CloseHandle(h); return (long)sz.QuadPart;
}
TC_CASE(size_zero_before_flush)
{
    FILE *f = fopen(F, "wb");
    CHECK(f != NULL); if (!f) return;
    setvbuf(f, NULL, _IOFBF, 4096);
    fprintf(f, "0123456789");
    CHECK_EQ_I(ext_size(), 0);
    fclose(f);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(size_zero_before_flush);
    remove(F); return tc_end();
}
