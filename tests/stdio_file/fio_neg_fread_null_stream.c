/* TC
 * id:    fio_neg_fread_null_stream
 * area:  stdio_file
 * kind:  negative
 * funcs: fread
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread: stream NULL -> EINVAL, invalid parameter handler
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

static FILE *volatile nul = NULL;
static size_t volatile v1 = 1;

TC_CASE(null_stream)
{
    char buf[4]; size_t r; int n0 = tc_iph_count; errno = 0;
    r = fread(buf, v1, v1, nul);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_stream); return tc_end(); }
