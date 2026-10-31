/* TC
 * id:    fio_neg_probe_fopen_directory
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen; Win32 CreateFile on a directory without FILE_FLAG_BACKUP_SEMANTICS
 * note:  Probes that opening the current directory as a file fails with NULL/EACCES.
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

TC_CASE(open_dot_for_write)
{
    FILE *f; errno = 0; f = fopen(".", "w"); CHECK(f == NULL); CHECK_EQ_I(errno, EACCES);
    if (f) fclose(f);
}
TC_CASE(open_dot_for_read)
{
    FILE *f; errno = 0; f = fopen(".", "r"); CHECK(f == NULL); CHECK_EQ_I(errno, EACCES);
    if (f) fclose(f);
}
int main(void) { tc_begin(); TC_RUN(open_dot_for_write); TC_RUN(open_dot_for_read); return tc_end(); }
