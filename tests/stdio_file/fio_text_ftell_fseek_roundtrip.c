/* TC
 * id:    fio_text_ftell_fseek_roundtrip
 * area:  stdio_file
 * funcs: fopen ftell fseek fgets fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.9.4 (ftell value usable with fseek SEEK_SET)
 * note:  only the ftell->fseek roundtrip is asserted; raw offsets are in fio_text_ftell_crlf_offset
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
#define F "tc_fio_text_ftell_fseek_roundtrip_" TC_CRT_DLL ".tmp"


TC_CASE(roundtrip)
{
    FILE *f; long pos[4]; char l[16]; int i;
    put(F, "ab\r\ncd\r\nef\r\n", 12);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (!f) { del(F); return; }
    pos[0] = ftell(f); CHECK(pos[0] >= 0);
    for (i = 1; i < 4; i++) { CHECK(fgets(l, 16, f) != NULL); pos[i] = ftell(f); }
    for (i = 0; i < 3; i++) {
        static const char *want[3] = { "ab\n", "cd\n", "ef\n" };
        CHECK_EQ_I(fseek(f, pos[i], SEEK_SET), 0);
        CHECK(fgets(l, 16, f) != NULL); CHECK_STR(l, want[i]);
    }
    fclose(f); del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(roundtrip);
    return tc_end();
}
