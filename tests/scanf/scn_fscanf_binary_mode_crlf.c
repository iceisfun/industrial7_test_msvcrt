/* TC
 * id:    scn_fscanf_binary_mode_crlf
 * area:  scanf
 * funcs: fscanf fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (binary mode: no translation); C11 7.4.1.10 (CR is whitespace)
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
#define F "tc_scn_fscanf_binary_mode_crlf_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(cr_kept)
{
    FILE *f; int a = 0, b = 0, r; char c1 = 'x', c2 = 'x'; wr(F, "1\r\n2", 4);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d%c%c%d", &a, &c1, &c2, &b); CHECK_EQ_I(r, 4); CHECK_EQ_I(a, 1); CHECK_EQ_I(c1, '\r'); CHECK_EQ_I(c2, '\n'); CHECK_EQ_I(b, 2);
    fclose(f); remove(F);
}
TC_CASE(s_stops_at_cr)
{
    FILE *f; char s[16] = ""; int r; wr(F, "abc\r\ndef", 8);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%15s", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "abc"); CHECK_EQ_I(fgetc(f), '\r');
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(cr_kept); TC_RUN(s_stops_at_cr); return tc_end(); }
