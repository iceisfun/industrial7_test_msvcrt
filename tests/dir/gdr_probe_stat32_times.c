/* TC
 * id:    gdr_probe_stat32_times
 * area:  dir
 * funcs: _stat32 _stat32i64
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _stat (st_mtime: time of last modification, UTC)
 * note:  PROBE: after SetFileTime(write = 2001-09-09T01:46:40Z) st_mtime must be 1000000000 independent of TZ and DST
 *        (old CRTs may have converted through local time); st_ctime (creation) nonzero. Kept apart from other stat tests.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_gdr_probe_stat32_times" TC_S(TC_CRT_VER) ".tmp"
static const char *volatile v_fn = FN;

static void setup(void)
{
    HANDLE h = CreateFileA(FN, GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    ULONGLONG t = (1000000000ULL + 11644473600ULL) * 10000000ULL;
    FILETIME ft;
    ft.dwLowDateTime = (DWORD)t; ft.dwHighDateTime = (DWORD)(t >> 32);
    if (h == INVALID_HANDLE_VALUE) return;
    SetFileTime(h, NULL, NULL, &ft);
    CloseHandle(h);
}
TC_CASE(stat32)
{
    struct _stat32 st; int r;
    setup();
    r = _stat32(v_fn, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK_EQ_I(st.st_mtime, 1000000000); CHECK(st.st_ctime != 0); }
}
TC_CASE(stat32i64)
{
    struct _stat32i64 st; int r;
    setup();
    r = _stat32i64(v_fn, &st);
    CHECK_EQ_I(r, 0);
    if (r == 0) { CHECK_EQ_I(st.st_mtime, 1000000000); CHECK(st.st_ctime != 0); }
}
int main(void) { tc_begin(); TC_RUN(stat32); TC_RUN(stat32i64); DeleteFileA(FN); return tc_end(); }
