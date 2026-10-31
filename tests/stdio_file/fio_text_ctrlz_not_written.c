/* TC
 * id:    fio_text_ctrlz_not_written
 * area:  stdio_file
 * funcs: fopen fwrite fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   write path does not interpret Ctrl-Z
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_text_ctrlz_not_written_" TC_CRT_DLL ".tmp"


TC_CASE(write_ctrlz)
{
    char b[16]; FILE *f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("a\x1a" "b", 1, 3, f), 3); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 3); CHECK_MEM(b, "a\x1a" "b", 3); del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(write_ctrlz);
    return tc_end();
}
