/* TC
 * id:    scn_neg_fscanf_rw_without_seek_probe
 * area:  scanf
 * kind:  negative
 * funcs: fscanf fopen fputs fseek fclose ferror
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.5.3p7 (input shall not directly follow output without fflush/fseek/fsetpos/rewind in update mode: UB); MS docs fopen
 * note:  Probes (cases without _control): after fputs on an "r+" stream, fscanf without intervening fseek/fflush returns EOF with ferror set (MS _filbuf rejects a stream in write state), errno left 0. The _control case is standard-conforming and must succeed.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_scn_neg_fscanf_rw_" TC_CRT_DLL ".tmp"
static void wfile(const char *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}

TC_CASE(write_then_scanf_no_seek_ret) { FILE *f; char s[16] = ""; int n = 0, r; wfile("123 456\n", 8); f = fopen(FN, "r+"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(fputs("zz", f) >= 0); r = fscanf(f, "%15s %d", s, &n); CHECK_EQ_I(r, EOF); CHECK(ferror(f) != 0); CHECK_EQ_I(n, 0); fclose(f); DeleteFileA(FN); }
TC_CASE(write_then_scanf_errno) { FILE *f; char s[16] = ""; int r; wfile("123 456\n", 8); f = fopen(FN, "r+"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(fputs("zz", f) >= 0); errno = 0; r = fscanf(f, "%15s", s); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, 0); fclose(f); DeleteFileA(FN); }
TC_CASE(write_flush_seek_control) { FILE *f; char s[16] = ""; int n = 0, r; wfile("123 456\n", 8); f = fopen(FN, "r+"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    CHECK(fputs("zz", f) >= 0); CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0); r = fscanf(f, "%15s %d", s, &n); CHECK_EQ_I(r, 2); CHECK_STR(s, "zz3"); CHECK_EQ_I(n, 456); CHECK_EQ_I(ferror(f), 0); fclose(f); DeleteFileA(FN); }

int main(void) { tc_begin(); TC_RUN(write_then_scanf_no_seek_ret); TC_RUN(write_then_scanf_errno); TC_RUN(write_flush_seek_control); return tc_end(); }
