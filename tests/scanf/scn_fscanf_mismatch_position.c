/* TC
 * id:    scn_fscanf_mismatch_position
 * area:  scanf
 * funcs: fscanf ftell fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.2p9 (at most one character pushed back); MS docs fscanf, ftell
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
#define F "tc_scn_fscanf_mismatch_position_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(no_match)
{
    FILE *f; int a = 5, r; wr(F, "abc", 3);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 5); CHECK_EQ_I(ftell(f), 0);
    fclose(f); remove(F);
}
TC_CASE(second_fails)
{
    FILE *f; int a = 0, b = 5, r; wr(F, "12abc", 5);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d%d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 5); CHECK_EQ_I(ftell(f), 2);
    fclose(f); remove(F);
}
TC_CASE(n_matches_ftell)
{
    FILE *f; int a = 0, n = -1, r; long before, after; wr(F, "  77 zz", 7);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) return;
    before = ftell(f);
    r = fscanf(f, "%d%n", &a, &n); after = ftell(f);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 4); CHECK_EQ_I(after - before, n);
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(no_match); TC_RUN(second_fails); TC_RUN(n_matches_ftell); return tc_end(); }
