/* TC
 * id:    scn_neg_fscanf_s_small_buf
 * area:  scanf
 * kind:  negative
 * funcs: fscanf_s fopen fclose
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fscanf_s: buffer too small fails the conversion
 * note:  Probes: fscanf_s %s with size 4 and token abcdef returns 0 and does not write beyond size 4 (case too_small); a[0] == 0 is the separately probed reset (case too_small_reset).
 */
#include "tc.h"
#include <stdio.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_neg_fscanf_" TC_CRT_DLL ".tmp"
TC_CASE(too_small) { FILE *f; char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; tc_iph_install(); wfile(FN, "abcdef\n", 7); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fscanf_s(f, "%s", a, (unsigned)4); fclose(f); DeleteFileA(FN); CHECK_EQ_I(n, 0); CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z'); }
TC_CASE(too_small_reset) { FILE *f; char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; tc_iph_install(); wfile(FN, "abcdef\n", 7); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; } n = fscanf_s(f, "%s", a, (unsigned)4); (void)n; fclose(f); DeleteFileA(FN); CHECK_EQ_I(a[0], 0); }
int main(void) { tc_begin(); TC_RUN(too_small); TC_RUN(too_small_reset); return tc_end(); }
