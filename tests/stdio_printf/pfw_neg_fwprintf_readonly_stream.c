/* TC
 * id:    pfw_neg_fwprintf_readonly_stream
 * area:  stdio_printf
 * funcs: fwprintf _wfopen
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fwprintf: returns a negative value on an output error
 * note:  Only the negative return and the unchanged file are asserted, not errno.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_ro_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(write_to_read_only_stream)
{
    FILE *f = fopen(FN, "wb"); FILE *r; int n; DWORD sz = 0; HANDLE h;
    CHECK(f != NULL); if (!f) return;
    fputc('Q', f); fclose(f);
    r = _wfopen(L"" FN, L"r"); CHECK(r != NULL); if (!r) { DeleteFileA(FN); return; }
    n = fwprintf(r, L"abc"); fflush(r); fclose(r);
    CHECK(n < 0);
    h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h != INVALID_HANDLE_VALUE) { sz = GetFileSize(h, NULL); CloseHandle(h); }
    CHECK_EQ_U(sz, 1);
    DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(write_to_read_only_stream); return tc_end(); }
