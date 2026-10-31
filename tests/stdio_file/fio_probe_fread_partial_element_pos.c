/* TC
 * id:    fio_probe_fread_partial_element_pos
 * area:  stdio_file
 * funcs: fread ftell fopen fclose
 * procs: 
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.8.1 (position indeterminate on partial element)
 * note:  PROBE: after fread(buf,4,3) on a 10-byte file (returns 2), is ftell == 10 (partial element consumed)? Believed true on MS CRT.
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

static const char *volatile fn = "tc_fio_probe_fread_partial_element_pos_" TC_CRT_DLL ".tmp";
static size_t volatile v3 = 3, v4 = 4;

TC_CASE(partial_pos)
{
    char buf[16]; FILE *f; mkfile(fn, "0123456789", 10); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    memset(buf, 0, sizeof buf);
    CHECK_EQ_U(fread(buf, v4, v3, f), 2);
    CHECK_EQ_I(ftell(f), 10);
    CHECK_MEM(buf, "0123456789", 10);
    fclose(f);
}
int main(void) { tc_begin(); TC_RUN(partial_pos); remove(fn); return tc_end(); }
