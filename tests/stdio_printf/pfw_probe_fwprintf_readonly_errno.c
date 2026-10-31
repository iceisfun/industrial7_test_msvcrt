/* TC
 * id:    pfw_probe_fwprintf_readonly_errno
 * area:  stdio_printf
 * funcs: fwprintf _wfopen
 * procs: 
 * crt:   >=80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs fwprintf
 * note:  PROBE: errno==EBADF after fwprintf to a stream opened "r". Negative return is asserted in pfw_neg_fwprintf_readonly_stream.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_probe_fwprintf_readonly_errno_" TC_S1(TC_CRT_VER) ".tmp"
static const wchar_t *volatile wname = L"" FN;
static const char *cname = FN;

TC_CASE(errno_ebadf)
{
    FILE *f; int n; DWORD w;
    HANDLE h = CreateFileA(cname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "x", 1, &w, NULL); CloseHandle(h); }
    f = _wfopen(wname, L"r");
    CHECK(f != NULL);
    if (f) {
        tc_iph_install(); tc_iph_count = 0; errno = 0;
        n = fwprintf(f, L"%d", 5);
        CHECK(n < 0);
        CHECK_EQ_I(errno, EBADF);
        fclose(f);
    }
    DeleteFileA(cname);
}
int main(void) { tc_begin(); TC_RUN(errno_ebadf); return tc_end(); }
