/* TC
 * id:    fio_fread_s_basic
 * area:  stdio_file
 * funcs: 
 * procs: fread_s fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread_s
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
static const char *volatile fn = "tc_fio_fread_s_basic_" TC_CRT_DLL ".tmp";
static size_t volatile z = 0, v1 = 1, v4 = 4, v3 = 3, v8 = 8, v16 = 16, v100 = 100;

TC_CASE(exact_fit)
{
    char buf[8]; FILE *f; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    mkfile(fn, "ABCDEFGH", 8); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(tc_p_fread_s(buf, v8, v4, 2, f), 2); CHECK_MEM(buf, "ABCDEFGH", 8); fclose(f);
}
TC_CASE(larger_buffer)
{
    char buf[32]; FILE *f; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(tc_p_fread_s(buf, sizeof buf, v1, v4, f), 4); CHECK_MEM(buf, "ABCD", 4); fclose(f);
}
TC_CASE(short_file)
{
    char buf[32]; FILE *f; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(tc_p_fread_s(buf, sizeof buf, v1, v16, f), 8); CHECK(feof(f) != 0);
    CHECK_MEM(buf, "ABCDEFGH", 8); fclose(f);
}
TC_CASE(zero)
{
    char buf[8]; FILE *f; int n0 = tc_iph_count; if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    CHECK_EQ_U(tc_p_fread_s(buf, sizeof buf, z, v3, f), 0);
    CHECK_EQ_U(tc_p_fread_s(buf, sizeof buf, v3, z, f), 0);
    CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count - n0, 0); CHECK_EQ_I(ftell(f), 0); fclose(f);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(fread_s); (void)v100;
    TC_RUN(exact_fit); TC_RUN(larger_buffer); TC_RUN(short_file); TC_RUN(zero);
    remove(fn); return tc_end();
}
