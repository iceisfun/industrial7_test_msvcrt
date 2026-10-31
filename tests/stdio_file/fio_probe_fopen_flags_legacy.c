/* TC
 * id:    fio_probe_fopen_flags_legacy
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fwrite remove
 * crt:   msvcr80 msvcr90 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen (flags table)
 * note:  Probes which of the MS-specific mode flags S R T D c N are accepted by msvcr80, msvcr90 and msvcrt.dll (each in its own case). Expectation: all accepted (fopen non-NULL, write works).
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
#define F "tc_fio_probe_fopen_flags_legacy_" TC_S1(TC_CRT_VER) ".tmp"

static void try_write(const char *mode)
{
    FILE *f = fopen(F, mode); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("z", 1, 1, f), 1); CHECK_EQ_I(fclose(f), 0); }
    remove(F);
}
TC_CASE(w_S) { try_write("wS"); }
TC_CASE(w_R) { try_write("wR"); }
TC_CASE(w_T) { try_write("wT"); }
TC_CASE(w_D) { try_write("wD"); }
TC_CASE(w_c) { try_write("wc"); }
TC_CASE(w_N) { try_write("wN"); }
int main(void) { tc_begin(); TC_RUN(w_S); TC_RUN(w_R); TC_RUN(w_T); TC_RUN(w_D); TC_RUN(w_c); TC_RUN(w_N); return tc_end(); }
