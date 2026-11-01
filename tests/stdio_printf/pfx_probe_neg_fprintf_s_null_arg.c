/* TC
 * id:    pfx_probe_neg_fprintf_s_null_arg
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf_s (secure variants validate arguments)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): fprintf_s with a NULL pointer for %s is NOT rejected: it prints "(null)" (return 6, errno untouched, no handler call);
 *        the file content is read back with kernel32. The old guess (-1/EINVAL/handler) was wrong.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_fprintf_s_null_arg_" TCV ".tmp"
static const char *volatile g_s_fmt = "%s";
static const char *volatile g_nullstr;

TC_CASE(null_string_arg)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf_s(f, g_s_fmt, g_nullstr);
    CHECK_EQ_I(r, 6); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
    { char buf[16]; DWORD got = 0; HANDLE h = CreateFileA(TMP, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
      CHECK(h != INVALID_HANDLE_VALUE); if (h != INVALID_HANDLE_VALUE) { ReadFile(h, buf, 15, &got, NULL); CloseHandle(h); } buf[got] = 0; CHECK_STR(buf, "(null)"); }
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(null_string_arg);
    return tc_end();
}
