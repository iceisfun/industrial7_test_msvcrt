/* TC
 * id:    pfw_probe_mixed_orientation_ansi
 * area:  stdio_printf
 * funcs: fwprintf fprintf _wfopen
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs fwprintf/fprintf
 * note:  PROBE: narrow and wide output calls interleaved on one ANSI (non-_O_WTEXT) stream all succeed; MS CRT has no stream orientation enforcement (unlike C99 fwide). File read back with kernel32.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_probe_mixed_orientation_ansi_" TC_S1(TC_CRT_VER) ".tmp"
static const wchar_t *volatile wname = L"" FN;
static const char *cname = FN;

TC_CASE(mixed)
{
    FILE *f; int a, b, c; char buf[64]; DWORD n = 0; HANDLE h; int i;
    f = _wfopen(wname, L"w");
    CHECK(f != NULL);
    if (!f) return;
    a = fprintf(f, "ab%d", 1);
    b = fwprintf(f, L"cd%d", 2);
    c = fprintf(f, "ef%d", 3);
    fclose(f);
    CHECK_EQ_I(a, 3); CHECK_EQ_I(b, 3); CHECK_EQ_I(c, 3);
    for (i = 0; i < 64; i++) buf[i] = 0;
    h = CreateFileA(cname, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 63, &n, NULL); CloseHandle(h); }
    CHECK_EQ_I(n, 9);
    CHECK_STR(buf, "ab1cd2ef3");
    DeleteFileA(cname);
}
int main(void) { tc_begin(); TC_RUN(mixed); return tc_end(); }
