/* TC
 * id:    fio_neg_ungetc_eof
 * area:  stdio_file
 * kind:  negative
 * funcs: ungetc fopen fgetc fclose ftell fread
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10p4 (ungetc: if c equals EOF the operation fails); MS docs ungetc
 * note:  iph assertion only on >=80 where the handler exists.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
#define F "tc_fio_neg_ungetc_eof_" V ".tmp"
static const char * volatile fname = F;
static volatile int eofv = EOF;
TC_CASE(mid_file)
{
    FILE *f; int n0 = tc_iph_count;
    put(fname, "abc", 3);
    f = fopen(fname, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(ftell(f), 1);
    CHECK_EQ_I(ungetc(eofv, f), EOF);
    CHECK_EQ_I(ftell(f), 1);
    CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(tc_iph_count - n0, 0);
    fclose(f);
}
TC_CASE(at_eof_keeps_indicator)
{
    FILE *f; char b[8];
    put(fname, "abc", 3);
    f = fopen(fname, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 8, f), 3);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc(eofv, f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f);
}
int main(void)
{
    tc_begin(); DeleteFileA(fname);
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(mid_file); TC_RUN(at_eof_keeps_indicator);
    DeleteFileA(fname);
    return tc_end();
}
