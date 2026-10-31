/* TC
 * id:    scn_fscanf_ungetc_pushback
 * area:  scanf
 * funcs: fscanf fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p9-10 (first non-matching char is left unread); MS docs fscanf
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
#define F "tc_scn_fscanf_ungetc_pushback_" TC_S1(TC_CRT_VER) ".tmp"

static FILE *open_with(const char *data, DWORD n) { wr(F, data, n); return fopen(F, "rb"); }

TC_CASE(int_stops)
{
    FILE *f = open_with("12,34", 5); int a = 0, r; CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(fgetc(f), ',');
    fclose(f); remove(F);
}
TC_CASE(literal_mismatch)
{
    FILE *f = open_with("abX", 3); int r; CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "abc"); CHECK_EQ_I(r, 0); CHECK_EQ_I(fgetc(f), 'X');
    fclose(f); remove(F);
}
TC_CASE(string_stops_at_ws)
{
    FILE *f = open_with("word next", 9); char s[16] = ""; int r; CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%15s", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "word"); CHECK_EQ_I(fgetc(f), ' ');
    fclose(f); remove(F);
}
TC_CASE(float_stops)
{
    FILE *f = open_with("1.5;", 4); double d = 0; int r; CHECK(f != NULL); if (!f) return;
    r = fscanf(f, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_NEAR(d, 1.5, 0); CHECK_EQ_I(fgetc(f), ';');
    fclose(f); remove(F);
}
int main(void) { tc_begin(); TC_RUN(int_stops); TC_RUN(literal_mismatch); TC_RUN(string_stops_at_ws); TC_RUN(float_stops); return tc_end(); }
