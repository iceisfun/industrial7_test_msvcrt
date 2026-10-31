/* TC
 * id:    fio_fmode_default_binary
 * area:  stdio_file
 * funcs: fopen fwrite fclose
 * procs: _set_fmode _get_fmode
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_fmode, _get_fmode, _fmode
 * note:  _set_fmode/_get_fmode go through procs: absent from the msvcr80 import lib. Assumes the exe starts with default fmode (0 or _O_TEXT).
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
#define F "tc_fio_fmode_default_binary_" TC_CRT_DLL ".tmp"


TC_DECL_PROC(int, _set_fmode, (int));
TC_DECL_PROC(int, _get_fmode, (int *));
#define TXT 0x4000
#define BIN 0x8000

TC_CASE(default_mode)
{
    int m = -1; char b[16]; FILE *f;
    if (!tc_p__get_fmode) TC_SKIP("no _get_fmode");
    CHECK_EQ_I(tc_p__get_fmode(&m), 0);
    CHECK(m == 0 || m == TXT);
    f = fopen(F, "w"); CHECK(f != NULL);
    if (f) { fwrite("a\n", 1, 2, f); fclose(f); }
    CHECK_EQ_I(slurp(F, b, 16), 3); CHECK_MEM(b, "a\r\n", 3); del(F);
}
TC_CASE(binary_default)
{
    int m = -1; char b[16]; FILE *f;
    if (!tc_p__set_fmode || !tc_p__get_fmode) TC_SKIP("no _set_fmode");
    CHECK_EQ_I(tc_p__set_fmode(BIN), 0);
    CHECK_EQ_I(tc_p__get_fmode(&m), 0); CHECK_EQ_I(m, BIN);
    f = fopen(F, "w"); CHECK(f != NULL);
    if (f) { fwrite("a\n", 1, 2, f); fclose(f); }
    CHECK_EQ_I(slurp(F, b, 16), 2); CHECK_MEM(b, "a\n", 2); del(F);
    f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { fwrite("a\n", 1, 2, f); fclose(f); }
    CHECK_EQ_I(slurp(F, b, 16), 3); CHECK_MEM(b, "a\r\n", 3); del(F);
    CHECK_EQ_I(tc_p__set_fmode(TXT), 0);
    f = fopen(F, "w"); CHECK(f != NULL);
    if (f) { fwrite("a\n", 1, 2, f); fclose(f); }
    CHECK_EQ_I(slurp(F, b, 16), 3); CHECK_MEM(b, "a\r\n", 3); del(F);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_fmode); TC_LOAD_PROC(_get_fmode);
    TC_RUN(default_mode);
    TC_RUN(binary_default);
    return tc_end();
}
