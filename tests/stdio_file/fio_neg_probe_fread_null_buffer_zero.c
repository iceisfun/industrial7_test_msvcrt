/* TC
 * id:    fio_neg_probe_fread_null_buffer_zero
 * area:  stdio_file
 * kind:  negative
 * funcs: fread fopen fclose
 * procs: 
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fread (ordering not documented)
 * note:  PROBE: whether the zero size/count shortcut precedes NULL-buffer validation. Believed: returns 0 with no invalid-parameter call.
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

static const char *volatile fn = "tc_fio_neg_probe_fread_null_buffer_zero_" TC_CRT_DLL ".tmp";
static void *volatile nul = NULL;
static size_t volatile z = 0, v4 = 4, v1 = 1;

TC_CASE(zero_count)
{
    FILE *f; int n0 = tc_iph_count; mkfile(fn, "abcd", 4);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(nul, v1, z, f), 0); CHECK_EQ_I(tc_iph_count - n0, 0); fclose(f);
}
TC_CASE(zero_size)
{
    FILE *f; int n0 = tc_iph_count;
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(nul, z, v4, f), 0); CHECK_EQ_I(tc_iph_count - n0, 0); fclose(f);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(zero_count); TC_RUN(zero_size); remove(fn); return tc_end(); }
