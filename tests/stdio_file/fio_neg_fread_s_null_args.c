/* TC
 * id:    fio_neg_fread_s_null_args
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fread_s fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread_s: NULL buffer/stream -> EINVAL; bufferSize smaller -> ERANGE
 * note:  bufferSize 0 case accepts ERANGE or EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void mkfile(const char *name, const void *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    if (n) WriteFile(h, data, n, &w, NULL);
    CloseHandle(h);
}
static DWORD fsize(const char *name)
{
    DWORD s = 0xffffffffu; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { s = GetFileSize(h, NULL); CloseHandle(h); }
    return s;
}

TC_DECL_PROC(size_t, fread_s, (void *, size_t, size_t, size_t, FILE *));
static const char *volatile fn = "tc_fio_neg_fread_s_null_args_" TC_CRT_DLL ".tmp";
static FILE *volatile nulf = NULL;
static void *volatile nulb = NULL;
static size_t volatile z = 0, v1 = 1, v4 = 4;

TC_CASE(null_stream)
{
    char buf[8]; size_t r; int n0 = tc_iph_count; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    errno = 0; r = tc_p_fread_s(buf, sizeof buf, v1, v4, nulf);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(null_buffer)
{
    FILE *f; size_t r; int n0 = tc_iph_count; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    mkfile(fn, "abcdefgh", 8); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = tc_p_fread_s(nulb, 8, v1, v4, f);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1); fclose(f);
}
TC_CASE(bufsize_zero)
{
    char buf[8]; FILE *f; size_t r; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = tc_p_fread_s(buf, z, v1, v4, f);
    CHECK_EQ_U(r, 0); CHECK(errno == ERANGE || errno == EINVAL); fclose(f);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(fread_s);
    TC_RUN(null_stream); TC_RUN(null_buffer); TC_RUN(bufsize_zero);
    remove(fn); return tc_end();
}
