/* TC
 * id:    scn_fscanf_ctrlz_text
 * area:  scanf
 * funcs: fscanf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen (text mode: Ctrl+Z is treated as end-of-file on input)
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
#define F "tc_scn_fscanf_ctrlz_text_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(text_mode_stops)
{
    FILE *f; int a = 0, b = 7, r; wr(F, "12 \x1a 34", 7);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12);
    r = fscanf(f, "%d", &b); CHECK_EQ_I(r, EOF); CHECK_EQ_I(b, 7);
    fclose(f); remove(F);
}
TC_CASE(binary_mode_reads_past)
{
    FILE *f; int a = 0, r; char c = 0; wr(F, "12 \x1a 34", 7);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12);
    r = fscanf(f, "%c", &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(c, ' ');
    r = fscanf(f, "%c", &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(c, 0x1a);
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 34);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(text_mode_stops); TC_RUN(binary_mode_reads_past); return tc_end(); }
