/* TC
 * id:    fio_wfopen_unicode_name
 * area:  stdio_file
 * kind:  positive
 * funcs: _wfopen fclose fwrite fread
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _wfopen
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

static const wchar_t WN[] = L"tc_fio_wfopen_unicode_name_\u00e9\u4e2d.tmp";

TC_CASE(unicode_name_roundtrip)
{
    char b[8]; FILE *f = _wfopen(WN, L"wb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("uni", 1, 3, f), 3); CHECK_EQ_I(fclose(f), 0); }
    f = _wfopen(WN, L"rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 8, f), 3); CHECK_MEM(b, "uni", 3); fclose(f); }
    DeleteFileW(WN);
}
TC_CASE(wide_mode_string)
{
    char b[8]; FILE *f = _wfopen(WN, L"wb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("a\nb", 1, 3, f), 3); CHECK_EQ_I(fclose(f), 0); }
    f = _wfopen(WN, L"rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 8, f), 3); CHECK_MEM(b, "a\nb", 3); fclose(f); }
    DeleteFileW(WN);
}
TC_CASE(exists_via_win32_and_wfopen)
{
    FILE *f = _wfopen(WN, L"wb"); CHECK(f != NULL);
    if (f) fclose(f);
    CHECK(GetFileAttributesW(WN) != INVALID_FILE_ATTRIBUTES);
    f = _wfopen(WN, L"r"); CHECK(f != NULL);
    if (f) fclose(f);
    DeleteFileW(WN);
    f = _wfopen(WN, L"r"); CHECK(f == NULL);
    if (f) fclose(f);
}
int main(void) { tc_begin(); TC_RUN(unicode_name_roundtrip); TC_RUN(wide_mode_string); TC_RUN(exists_via_win32_and_wfopen); return tc_end(); }
