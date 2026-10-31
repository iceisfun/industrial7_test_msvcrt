/* TC
 * id:    fio_probe_fopen_mode_spaces
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (ccs, mode string)
 * note:  Probes whether whitespace after/inside the mode string is skipped by the CRT mode parser, and whether the documented 'r, ccs=UTF-8' comma+space form is accepted.
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
static int slurp(const char *n, char *b, int cap)   /* file size via kernel32, -1 if missing */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static void del(const char *n) { DeleteFileA(n); }
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_fio_probe_fopen_mode_spaces_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(trailing_space)
{
    FILE *f; put(F, "x", 1);
    f = fopen(F, "r "); CHECK(f != NULL); if (f) fclose(f); del(F);
}
TC_CASE(space_inside)
{
    FILE *f = fopen(F, "w b"); CHECK(f != NULL); if (f) fclose(f); del(F);
}
TC_CASE(comma_ccs_utf8)
{
    FILE *f; put(F, "", 0);
    f = fopen(F, "r, ccs=UTF-8"); CHECK(f != NULL); if (f) fclose(f); del(F);
}
int main(void) { tc_begin(); TC_RUN(trailing_space); TC_RUN(space_inside); TC_RUN(comma_ccs_utf8); return tc_end(); }
