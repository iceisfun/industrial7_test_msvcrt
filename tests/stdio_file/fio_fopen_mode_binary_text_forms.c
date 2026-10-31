/* TC
 * id:    fio_fopen_mode_binary_text_forms
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fwrite fread
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (t and b mode flags)
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
static int slurp(const char *n, char *b, int cap)   /* file size via kernel32, -1 if missing */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static void del(const char *n) { DeleteFileA(n); }
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_fio_fopen_mode_binary_text_forms_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(binary_no_translation)
{
    static const char *const modes[3] = { "wb", "w+b", "wb+" };
    int k;
    for (k = 0; k < 3; k++) {
        char b[16]; FILE *f = fopen(F, modes[k]); CHECK(f != NULL);
        if (f) { CHECK_EQ_U(fwrite("a\nb", 1, 3, f), 3); CHECK_EQ_I(fclose(f), 0); }
        CHECK_EQ_I(slurp(F, b, 16), 3); CHECK_MEM(b, "a\nb", 3);
    }
    del(F);
}
TC_CASE(text_write_crlf)
{
    char b[16]; FILE *f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("a\nb", 1, 3, f), 3); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 4); CHECK_MEM(b, "a\r\nb", 4); del(F);
}
TC_CASE(read_back_rb_rt)
{
    char b[16]; FILE *f; put(F, "a\r\nb", 4);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 4); CHECK_MEM(b, "a\r\nb", 4); fclose(f); }
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 3); CHECK_MEM(b, "a\nb", 3); fclose(f); }
    del(F);
}
TC_CASE(rplusb_equals_rbplus)
{
    static const char *const modes[2] = { "r+b", "rb+" };
    int k;
    for (k = 0; k < 2; k++) {
        char b[16]; FILE *f; put(F, "a\nb", 3);
        f = fopen(F, modes[k]); CHECK(f != NULL);
        if (f) {
            CHECK_EQ_U(fread(b, 1, 3, f), 3); CHECK_MEM(b, "a\nb", 3);
            rewind(f); CHECK_EQ_U(fwrite("X\n", 1, 2, f), 2); CHECK_EQ_I(fclose(f), 0);
        }
        CHECK_EQ_I(slurp(F, b, 16), 3); CHECK_MEM(b, "X\nb", 3);
    }
    del(F);
}
int main(void) { tc_begin(); TC_RUN(binary_no_translation); TC_RUN(text_write_crlf); TC_RUN(read_back_rb_rt); TC_RUN(rplusb_equals_rbplus); return tc_end(); }
