/* TC
 * id:    pfx_fprintf_setmode_switch
 * area:  stdio_printf
 * funcs: fprintf fopen _fileno _setmode fflush fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _setmode (returns previous mode; _O_BINARY disables CR-LF translation), _fileno, fflush
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#define F "tc_pfx_fprintf_setmode_switch_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(text_to_binary)
{
    FILE *f = fopen(F, "w"); int fd, prev; long sz;
    CHECK(f != NULL); if (!f) return;
    fd = _fileno(f);
    CHECK(fd >= 0);
    fprintf(f, "a\n");
    CHECK_EQ_I(fflush(f), 0);
    prev = _setmode(fd, _O_BINARY);
    CHECK_EQ_I(prev, _O_TEXT);
    CHECK_EQ_I(prev, 0x4000);
    fprintf(f, "b\n");
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 5);
    CHECK_MEM(tc_buf, "a\r\nb\n", 5);
}
TC_CASE(binary_to_text_returns_binary)
{
    FILE *f = fopen(F, "wb"); int fd, prev; long sz;
    CHECK(f != NULL); if (!f) return;
    fd = _fileno(f);
    fprintf(f, "a\n");
    CHECK_EQ_I(fflush(f), 0);
    prev = _setmode(fd, _O_TEXT);
    CHECK_EQ_I(prev, _O_BINARY);
    CHECK_EQ_I(prev, 0x8000);
    fprintf(f, "b\n");
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 5);
    CHECK_MEM(tc_buf, "a\nb\r\n", 5);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(text_to_binary); TC_RUN(binary_to_text_returns_binary);
    remove(F); return tc_end();
}
