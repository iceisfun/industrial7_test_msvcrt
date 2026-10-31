/* TC
 * id:    scn_fscanf_long_line
 * area:  scanf
 * funcs: fscanf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.21.6.2
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
static void wr(const char *name, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, d, n, &w, NULL); CloseHandle(h);
}
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_scn_fscanf_long_line_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(big_string)
{
    FILE *f; char *in = (char *)malloc(100001), *out = (char *)malloc(100002); int r, i, bad = 0;
    CHECK(in && out); if (!in || !out) return;
    for (i = 0; i < 100000; i++) in[i] = (char)('a' + i % 26);
    in[100000] = 0; wr(F, in, 100000);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) {
        r = fscanf(f, "%100001s", out); CHECK_EQ_I(r, 1);
        for (i = 0; i < 100000; i++) if (out[i] != in[i]) bad++;
        CHECK_EQ_I(bad, 0); CHECK_EQ_I(out[100000], 0);
        fclose(f);
    }
    remove(F); free(in); free(out);
}
TC_CASE(many_ints)
{
    FILE *f; char *buf = (char *)malloc(80000); int i, len = 0, v, r, cnt = 0; long long sum = 0;
    CHECK(buf != NULL); if (!buf) return;
    for (i = 1; i <= 10000; i++) {
        int x = i, d = 0; char t[8];
        while (x) { t[d++] = (char)('0' + x % 10); x /= 10; }
        while (d) buf[len++] = t[--d];
        buf[len++] = (i % 10) ? ' ' : '\n';
    }
    wr(F, buf, (DWORD)len);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) {
        while ((r = fscanf(f, "%d", &v)) == 1) { sum += v; cnt++; }
        CHECK_EQ_I(cnt, 10000); CHECK_EQ_I(sum, 50005000LL);
        fclose(f);
    }
    remove(F); free(buf);
}
int main(void) { tc_begin(); TC_RUN(big_string); TC_RUN(many_ints); return tc_end(); }
