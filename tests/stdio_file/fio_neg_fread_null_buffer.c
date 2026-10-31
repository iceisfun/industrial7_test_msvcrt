/* TC
 * id:    fio_neg_fread_null_buffer
 * area:  stdio_file
 * kind:  negative
 * funcs: fread fopen fclose
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread: buffer NULL -> EINVAL, invalid parameter handler
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

static const char *volatile fn = "tc_fio_neg_fread_null_buffer_" TC_CRT_DLL ".tmp";
static void *volatile nul = NULL;
static size_t volatile v1 = 1, v4 = 4;

TC_CASE(null_buffer)
{
    FILE *f; size_t r; int n0 = tc_iph_count; mkfile(fn, "abcdefgh", 8);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = fread(nul, v1, v4, f);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
    fclose(f);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buffer); remove(fn); return tc_end(); }
