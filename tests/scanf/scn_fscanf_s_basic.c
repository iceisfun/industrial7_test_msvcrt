/* TC
 * id:    scn_fscanf_s_basic
 * area:  scanf
 * funcs: fscanf_s fopen fclose
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fscanf_s, _fscanf_s_l; size argument follows %s/%c/%[ pointers
 */
#include "tc.h"
#include <stdio.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_fscanf_s_" TC_CRT_DLL ".tmp"
static FILE *openf(const char *txt, unsigned n) { wfile(FN, txt, n); return fopen(FN, "r"); }
TC_CASE(s_size) { FILE *f = openf("hello world\n", 12); char a[8], b[6]; int n; CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fscanf_s(f, "%s %s", a, (unsigned)8, b, (unsigned)6); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 2); CHECK_STR(a, "hello"); CHECK_STR(b, "world"); }
TC_CASE(c_size) { FILE *f = openf("xyz", 3); char a[4]; int n, i; for (i = 0; i < 4; i++) a[i] = 'Z'; CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fscanf_s(f, "%2c", a, (unsigned)2); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[0], 'x'); CHECK_EQ_I(a[1], 'y'); CHECK_EQ_I(a[2], 'Z'); }
TC_CASE(numbers) { FILE *f = openf("10 0x1F 2.5 end\n", 16); int i = 0, h = 0, n; double d = 0; char s[4]; CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fscanf_s(f, "%d %i %lf %s", &i, &h, &d, s, (unsigned)4); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 4); CHECK_EQ_I(i, 10); CHECK_EQ_I(h, 31); CHECK_EQ_I(d == 2.5, 1); CHECK_STR(s, "end"); }
TC_CASE(eof) { FILE *f = openf("", 0); char a[4]; int n; CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fscanf_s(f, "%s", a, (unsigned)4); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, EOF); }
int main(void) { tc_begin(); TC_RUN(s_size); TC_RUN(c_size); TC_RUN(numbers); TC_RUN(eof); return tc_end(); }
