/* TC
 * id:    scn_neg_fscanf_closed_fd_underlying_probe
 * area:  scanf
 * kind:  negative
 * funcs: fscanf fwscanf fopen _fileno _close fclose ferror
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _close, _read (EBADF on invalid fd; invalid-parameter handler invoked)
 * note:  Probes fscanf/fwscanf on a FILE whose descriptor was closed behind its back with _close(_fileno(f)): unbuffered input must fail with EOF, errno EBADF, ferror set, outputs untouched. buffered_data_still_served_probe: when the data was already buffered (fgetc+ungetc), fscanf is believed to still be served from the buffer. The later fclose may report an error; ignored.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <io.h>
#include <errno.h>

#define FN "tc_scn_neg_fscanf_cfd_" TC_CRT_DLL ".tmp"
static void wfile(const char *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
static FILE *openclosed(void)
{
    FILE *f; wfile("123\n", 4); f = fopen(FN, "r");
    if (f) { setvbuf(f, NULL, _IONBF, 0); _close(_fileno(f)); }
    return f;
}
static void done(FILE *f) { if (f) fclose(f); DeleteFileA(FN); }

TC_CASE(fscanf_after_close_ret_eof) { FILE *f; int a = 99, r; tc_iph_install(); f = openclosed(); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); done(f); }
TC_CASE(errno_ebadf) { FILE *f; int a = 99, r; tc_iph_install(); f = openclosed(); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } errno = 0; r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EBADF); done(f); }
TC_CASE(fwscanf_after_close) { FILE *f; int a = 99, r; tc_iph_install(); f = openclosed(); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } errno = 0; r = fwscanf(f, L"%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); CHECK_EQ_I(errno, EBADF); done(f); }
TC_CASE(ferror_set) { FILE *f; int a = 99, r; tc_iph_install(); f = openclosed(); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK(ferror(f) != 0); done(f); }
TC_CASE(buffered_data_still_served_probe) { FILE *f; int a = 99, r, c; tc_iph_install(); wfile("123\n", 4); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    c = fgetc(f); CHECK_EQ_I(c, '1'); CHECK_EQ_I(ungetc(c, f), '1'); _close(_fileno(f)); r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 123); done(f); }

int main(void) { tc_begin(); TC_RUN(fscanf_after_close_ret_eof); TC_RUN(errno_ebadf); TC_RUN(fwscanf_after_close); TC_RUN(ferror_set); TC_RUN(buffered_data_still_served_probe); return tc_end(); }
