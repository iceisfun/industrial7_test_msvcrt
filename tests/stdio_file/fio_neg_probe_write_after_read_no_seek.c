/* TC
 * id:    fio_neg_probe_write_after_read_no_seek
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fgetc fputc fclose ferror clearerr setvbuf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.5.3p7 (undefined); MS _flsbuf legacy
 * note:  PROBE: on "r+b" stream, fgetc then fputc with no fseek/fflush. Case not_at_eof uses an UNBUFFERED stream (_IONBF)
 *        so the putc fast path (--_cnt >= 0) cannot silently store into the read buffer (which a buffered stream would do);
 *        expected: _flsbuf sees _IOREAD without _IOEOF -> fputc returns EOF and sets ferror. Case at_eof: after a read hit EOF the
 *        fputc succeeds. Cases kept separate.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <stdio.h>

static void mkfile(const char *name, const char *data, unsigned n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    if (n) WriteFile(h, data, n, &w, NULL);
    CloseHandle(h);
}
static long long fsize(const char *name)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(name, GetFileExInfoStandard, &d)) return -1;
    return ((long long)d.nFileSizeHigh << 32) | d.nFileSizeLow;
}

static const char * volatile fname = "tc_fio_neg_probe_write_after_read_no_seek_a" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(not_at_eof)
{
    FILE *fp; int c;
    fp = fopen(fname, "r+b"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(setvbuf(fp, NULL, _IONBF, 0), 0);
    CHECK_EQ_I(fgetc(fp), '0');
    c = fputc('X', fp);
    CHECK_EQ_I(c, EOF);
    CHECK(ferror(fp) != 0);
    clearerr(fp);
    fclose(fp);
}
TC_CASE(at_eof)
{
    FILE *fp; int i, c;
    fp = fopen(fname, "r+b"); CHECK(fp != NULL); if (!fp) return;
    for (i = 0; i < 10; i++) fgetc(fp);
    CHECK_EQ_I(fgetc(fp), EOF);
    c = fputc('Y', fp);
    CHECK_EQ_I(c, 'Y');
    CHECK_EQ_I(ferror(fp), 0);
    fclose(fp);
}
int main(void)
{
    tc_begin();
    mkfile(fname, "0123456789", 10);
    TC_RUN(not_at_eof);
    mkfile(fname, "0123456789", 10);
    TC_RUN(at_eof);
    DeleteFileA(fname);
    return tc_end();
}
