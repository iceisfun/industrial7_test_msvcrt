/* TC
 * id:    fio_ungetc_basic
 * area:  stdio_file
 * funcs: ungetc fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_ungetc_basic_" TC_CRT_DLL ".tmp"

TC_CASE(same_char)
{
    FILE *f; put(F, "abc", 3);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(ungetc('a', f), 'a'); CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); fclose(f); }
    del(F);
}
TC_CASE(other_char)
{
    FILE *f; put(F, "abc", 3);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(ungetc('Z', f), 'Z'); CHECK_EQ_I(fgetc(f), 'Z'); CHECK_EQ_I(fgetc(f), 'b'); fclose(f); }
    del(F);
}
TC_CASE(high_byte)
{
    FILE *f; put(F, "abc", 3);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(ungetc(0xFF, f), 255); CHECK_EQ_I(fgetc(f), 255); fclose(f); }
    del(F);
}
int main(void) { tc_begin(); TC_RUN(same_char); TC_RUN(other_char); TC_RUN(high_byte); return tc_end(); }
