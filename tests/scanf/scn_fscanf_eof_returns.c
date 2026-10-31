/* TC
 * id:    scn_fscanf_eof_returns
 * area:  scanf
 * funcs: fscanf fopen fclose feof
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16 (EOF if input failure before first conversion); MS docs fscanf return value
 */
#include "tc.h"
#include <stdio.h>
static void wr(const char *name, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, d, n, &w, NULL); CloseHandle(h);
}
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_scn_fscanf_eof_returns_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(empty)
{
    FILE *f; int a = 7, r;
    wr(F, "", 0);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK(feof(f) != 0); CHECK_EQ_I(a, 7);
    fclose(f); remove(F);
}
TC_CASE(ws_only)
{
    FILE *f; int a = 7, r;
    wr(F, " \t\n  \n", 6);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 7);
    fclose(f); remove(F);
}
TC_CASE(second_call)
{
    FILE *f; int a = 0, b = 7, r;
    wr(F, "42", 2);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 42);
    r = fscanf(f, "%d", &b); CHECK_EQ_I(r, EOF); CHECK_EQ_I(b, 7);
    fclose(f); remove(F);
}
TC_CASE(partial)
{
    FILE *f; int a = 0, b = 7, r;
    wr(F, "1", 1);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 7);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(empty); TC_RUN(ws_only); TC_RUN(second_call); TC_RUN(partial); return tc_end(); }
