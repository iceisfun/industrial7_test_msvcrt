/* TC
 * id:    scn_fscanf_text_mode_crlf
 * area:  scanf
 * funcs: fscanf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: CR-LF translated to LF on input)
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
#define F "tc_scn_fscanf_text_mode_crlf_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(c_is_lf)
{
    FILE *f; int a = 0, b = 0, r; char c = 'x'; wr(F, "1\r\n2", 4);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d%c%d", &a, &c, &b); CHECK_EQ_I(r, 3); CHECK_EQ_I(a, 1); CHECK_EQ_I(c, '\n'); CHECK_EQ_I(b, 2);
    fclose(f); remove(F);
}
TC_CASE(scanset_no_cr)
{
    FILE *f; char s[16] = "", t[16] = ""; int r; wr(F, "abc\r\ndef\r\n", 10);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%15[^\n]", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "abc");
    r = fscanf(f, "%*c%15[^\n]", t); CHECK_EQ_I(r, 1); CHECK_STR(t, "def");
    fclose(f); remove(F);
}
TC_CASE(line_count)
{
    FILE *f; int lines = 0, v, r; wr(F, "1\r\n2\r\n3\r\n", 9);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    while ((r = fscanf(f, "%d%*[\n]", &v)) == 1) lines++;
    CHECK_EQ_I(lines, 3);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(c_is_lf); TC_RUN(scanset_no_cr); TC_RUN(line_count); return tc_end(); }
