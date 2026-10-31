/* TC
 * id:    fio_text_crlf_buffer_boundary
 * area:  stdio_file
 * funcs: fopen setvbuf fgetc fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen / setvbuf; translation must be correct across buffer refills
 * note:  setvbuf sizes below 2 are invalid; 5/7/16 are legal. A caller-supplied static buffer is used (NULL-buffer behaviour is probed elsewhere). Lines are 'x\r\n' (3 raw bytes) so 900 raw bytes.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_text_crlf_buffer_boundary_" TC_CRT_DLL ".tmp"


static char sbuf[32];
static void run(size_t bs)
{
    char raw[900]; int i, c, lines = 0, crs = 0; FILE *f;
    for (i = 0; i < 300; i++) { raw[i*3] = 'x'; raw[i*3+1] = '\r'; raw[i*3+2] = '\n'; }
    put(F, raw, 900);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (!f) { del(F); return; }
    CHECK_EQ_I(setvbuf(f, sbuf, _IOFBF, bs), 0);
    while ((c = fgetc(f)) != EOF) { if (c == '\r') crs++; if (c == '\n') lines++; }
    CHECK_EQ_I(crs, 0); CHECK_EQ_I(lines, 300);
    fclose(f); del(F);
}
TC_CASE(buf5)  { run(5); }
TC_CASE(buf7)  { run(7); }
TC_CASE(buf16) { run(16); }
int main(void)
{
    tc_begin();
    TC_RUN(buf5);
    TC_RUN(buf7);
    TC_RUN(buf16);
    return tc_end();
}
