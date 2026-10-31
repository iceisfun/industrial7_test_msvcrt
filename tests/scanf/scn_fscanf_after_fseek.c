/* TC
 * id:    scn_fscanf_after_fseek
 * area:  scanf
 * funcs: fscanf fseek fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.9.2 fseek, 7.21.6.2
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
#define F "tc_scn_fscanf_after_fseek_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(rescan)
{
    FILE *f; int a = 0, b = 0, r; wr(F, "123 456", 7);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 123);
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    r = fscanf(f, "%d", &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(b, 123);
    fclose(f); remove(F);
}
TC_CASE(seek_end_eof)
{
    FILE *f; int a = 9, r; wr(F, "123 456", 7);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fseek(f, 0, SEEK_END), 0);
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 9);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(rescan); TC_RUN(seek_end_eof); return tc_end(); }
