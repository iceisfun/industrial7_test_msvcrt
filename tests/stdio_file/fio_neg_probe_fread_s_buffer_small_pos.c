/* TC
 * id:    fio_neg_probe_fread_s_buffer_small_pos
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fread_s fopen fclose
 * crt:   >=80
 * tier:  p2
 * conf:  observed
 * ref:   MS docs fread_s: bufferSize too small -> ERANGE
 * note:  PROBE: file position after a rejected fread_s (bufferSize too small). Believed unchanged (check precedes reading); not documented.
 *        OBSERVED (Win10 22H2, class A): fread_s(buf,4,1,8,f) on "abcdefghijkl" returns 0, errno ERANGE (34), handler called once, and the position advanced to 1.
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
static const char *volatile fn = "tc_fio_neg_probe_fread_s_buffer_small_pos_" TC_CRT_DLL ".tmp";
static size_t volatile v1 = 1, v4 = 4, v8 = 8;

TC_CASE(too_small)
{
    char buf[16]; FILE *f; size_t r; int n0 = tc_iph_count;
    if (!tc_p_fread_s) TC_SKIP("fread_s not exported");
    mkfile(fn, "abcdefghijkl", 12); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = tc_p_fread_s(buf, v4, v1, v8, f);
    CHECK_EQ_U(r, 0);
    CHECK_EQ_I(errno, ERANGE);
    CHECK(tc_iph_count - n0 >= 1);
    CHECK_EQ_I(ftell(f), 1);    /* observed: one byte was consumed before the size check failed (raw: ftell(f) == 0 got=1) */
    CHECK_EQ_I(fgetc(f), 'b');
    fclose(f);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(fread_s); TC_RUN(too_small); remove(fn); return tc_end(); }
