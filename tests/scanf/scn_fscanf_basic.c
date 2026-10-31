/* TC
 * id:    scn_fscanf_basic
 * area:  scanf
 * funcs: fscanf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2; MS docs fscanf
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
#define F "tc_scn_fscanf_basic_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(lines)
{
    FILE *f; int a = 0, b = 0, r; char s[16] = "", t[16] = "";
    wr(F, "10 hello\n20 world\n", 18);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d %15s", &a, s); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 10); CHECK_STR(s, "hello");
    r = fscanf(f, "%d %15s", &b, t); CHECK_EQ_I(r, 2); CHECK_EQ_I(b, 20); CHECK_STR(t, "world");
    fclose(f); remove(F);
}
TC_CASE(c_reads_newline)
{
    FILE *f; int a = 0, b = 0, r; char c = 'x';
    wr(F, "1\n2", 3);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d%c%d", &a, &c, &b); CHECK_EQ_I(r, 3); CHECK_EQ_I(a, 1); CHECK_EQ_I(c, '\n'); CHECK_EQ_I(b, 2);
    fclose(f); remove(F);
}
TC_CASE(literal)
{
    FILE *f; int a = 0, b = 0, r;
    wr(F, "x=5,y=6", 7);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "x=%d,y=%d", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 6);
    fclose(f); remove(F);
}
TC_CASE(percent_n)
{
    FILE *f; int a = 0, n = -1, r;
    wr(F, "  345 rest", 10);
    f = fopen(F, "r"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d%n", &a, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 345); CHECK_EQ_I(n, 5);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(lines); TC_RUN(c_reads_newline); TC_RUN(literal); TC_RUN(percent_n); return tc_end(); }
