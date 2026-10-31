/* TC
 * id:    sec_fopen_s_basic
 * area:  secure_crt
 * funcs:
 * procs: fopen_s _wfopen_s fclose fwrite fread
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s, _wfopen_s: returns zero on success and sets *pFile
 * note:  temp files tc_sec_fopen_*.tmp in cwd, removed at the end of each case.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *fopen_s_t)(void **, const char *, const char *);
typedef int (__cdecl *wfopen_s_t)(void **, const wchar_t *, const wchar_t *);
typedef int (__cdecl *fclose_t)(void *);
typedef size_t (__cdecl *fwrite_t)(const void *, size_t, size_t, void *);
typedef size_t (__cdecl *fread_t)(void *, size_t, size_t, void *);
static fopen_s_t p_fopen_s; static wfopen_s_t p_wfopen_s; static fclose_t p_fclose; static fwrite_t p_fwrite; static fread_t p_fread;

static DWORD file_size(const char *n)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s; if (h == INVALID_HANDLE_VALUE) return 0xffffffff;
    s = GetFileSize(h, NULL); CloseHandle(h); return s;
}

TC_CASE(create_write_readback)
{
    void *f = NULL; char b[8] = { 0 }; int r;
    DeleteFileA("tc_sec_fopen_a.tmp");
    r = p_fopen_s(&f, "tc_sec_fopen_a.tmp", "wb"); CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(p_fwrite("hello", 1, 5, f), 5); CHECK_EQ_I(p_fclose(f), 0); }
    CHECK_EQ_U(file_size("tc_sec_fopen_a.tmp"), 5);
    f = NULL; r = p_fopen_s(&f, "tc_sec_fopen_a.tmp", "rb"); CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(p_fread(b, 1, 7, f), 5); CHECK_STR(b, "hello"); p_fclose(f); }
    DeleteFileA("tc_sec_fopen_a.tmp");
}
TC_CASE(append_mode)
{
    void *f = NULL; int r;
    DeleteFileA("tc_sec_fopen_b.tmp");
    r = p_fopen_s(&f, "tc_sec_fopen_b.tmp", "wb"); CHECK_EQ_I(r, 0);
    if (f) { p_fwrite("abc", 1, 3, f); p_fclose(f); }
    f = NULL; r = p_fopen_s(&f, "tc_sec_fopen_b.tmp", "ab"); CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (f) { p_fwrite("def", 1, 3, f); p_fclose(f); }
    CHECK_EQ_U(file_size("tc_sec_fopen_b.tmp"), 6);
    DeleteFileA("tc_sec_fopen_b.tmp");
}
TC_CASE(binary_mode_rb)
{
    void *f = NULL; unsigned char d[4] = { 0x0d, 0x0a, 0x1a, 0x41 }, o[8] = { 0 }; int r;
    DeleteFileA("tc_sec_fopen_c.tmp");
    r = p_fopen_s(&f, "tc_sec_fopen_c.tmp", "wb"); CHECK_EQ_I(r, 0);
    if (f) { CHECK_EQ_U(p_fwrite(d, 1, 4, f), 4); p_fclose(f); }
    CHECK_EQ_U(file_size("tc_sec_fopen_c.tmp"), 4);   /* no newline translation, no Ctrl-Z handling */
    f = NULL; r = p_fopen_s(&f, "tc_sec_fopen_c.tmp", "rb"); CHECK_EQ_I(r, 0);
    if (f) { CHECK_EQ_U(p_fread(o, 1, 8, f), 4); CHECK_MEM(o, d, 4); p_fclose(f); }
    DeleteFileA("tc_sec_fopen_c.tmp");
}
TC_CASE(wfopen_s_wide_name)
{
    void *f = NULL; int r;
    if (!p_wfopen_s) TC_SKIP("_wfopen_s not exported");
    DeleteFileA("tc_sec_fopen_w.tmp");
    r = p_wfopen_s(&f, L"tc_sec_fopen_w.tmp", L"wb"); CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (f) { p_fwrite("xyz", 1, 3, f); p_fclose(f); }
    CHECK_EQ_U(file_size("tc_sec_fopen_w.tmp"), 3);
    DeleteFileA("tc_sec_fopen_w.tmp");
}
TC_CASE(pfile_set_nonnull)
{
    void *f = (void *)(ULONG_PTR)0x1; int r;
    DeleteFileA("tc_sec_fopen_d.tmp");
    r = p_fopen_s(&f, "tc_sec_fopen_d.tmp", "w"); CHECK_EQ_I(r, 0);
    CHECK(f != NULL); CHECK(f != (void *)(ULONG_PTR)0x1);
    if (f) p_fclose(f);
    DeleteFileA("tc_sec_fopen_d.tmp");
}
int main(void)
{
    tc_begin();
    p_fopen_s = (fopen_s_t)tc_crt_proc("fopen_s"); p_wfopen_s = (wfopen_s_t)tc_crt_proc("_wfopen_s");
    p_fclose = (fclose_t)tc_crt_proc("fclose"); p_fwrite = (fwrite_t)tc_crt_proc("fwrite"); p_fread = (fread_t)tc_crt_proc("fread");
    CHECK(p_fopen_s && p_fclose && p_fwrite && p_fread);
    if (!(p_fopen_s && p_fclose && p_fwrite && p_fread)) return tc_end();
    TC_RUN(create_write_readback); TC_RUN(append_mode); TC_RUN(binary_mode_rb); TC_RUN(wfopen_s_wide_name); TC_RUN(pfile_set_nonnull);
    return tc_end();
}
