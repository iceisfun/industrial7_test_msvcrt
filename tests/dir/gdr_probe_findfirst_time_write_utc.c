/* TC
 * id:    gdr_probe_findfirst_time_write_utc
 * area:  dir
 * funcs: _wfindfirst64 _findclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _wfindfirst: time fields are time_t values
 * note:  PROBE: that the find* time fields equal UTC epoch seconds of the file's FILETIME (the CRT converts via local time
 *        using the CURRENT zone/DST state, so a zone with DST tested outside the DST season may be off by 3600 s; that is part of
 *        what is measured). FILETIMEs set: write 2001-09-09T01:46:40Z (1000000000), create +100, access +200.
 *        Create/access checks only run on NTFS.
 */
#include "tc.h"
#include <io.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define F "tc_gdr_probe_time_utc" TC_S(TC_CRT_VER) ".tmp"
#define W2(x) L##x
#define W(x) W2(x)
static const wchar_t *volatile v_spec;

static void to_ft(long long secs, FILETIME *ft)
{
    unsigned long long t = (unsigned long long)secs * 10000000ULL + 116444736000000000ULL;
    ft->dwLowDateTime = (DWORD)t; ft->dwHighDateTime = (DWORD)(t >> 32);
}
static int is_ntfs(void)
{
    wchar_t fs[32]; fs[0] = 0;
    if (!GetVolumeInformationW(NULL, NULL, 0, NULL, NULL, NULL, fs, 32)) return 0;
    return fs[0] == 'N' && fs[1] == 'T' && fs[2] == 'F' && fs[3] == 'S' && !fs[4];
}
static int make(void)
{
    HANDLE h = CreateFileA(F, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    FILETIME c, a, w; int ok;
    if (h == INVALID_HANDLE_VALUE) return 0;
    to_ft(1000000100LL, &c); to_ft(1000000200LL, &a); to_ft(1000000000LL, &w);
    ok = SetFileTime(h, &c, &a, &w);
    CloseHandle(h);
    return ok;
}

TC_CASE(write_time)
{
    struct _wfinddata64_t fd; intptr_t h;
    if (!make()) { DeleteFileA(F); TC_SKIP("cannot set file times"); }
    v_spec = W(F);
    h = _wfindfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(fd.time_write, 1000000000LL); _findclose(h); }
    DeleteFileA(F);
}
TC_CASE(create_access_time)
{
    struct _wfinddata64_t fd; intptr_t h;
    if (!is_ntfs()) TC_SKIP("not NTFS");
    if (!make()) { DeleteFileA(F); TC_SKIP("cannot set file times"); }
    v_spec = W(F);
    h = _wfindfirst64(v_spec, &fd);
    CHECK(h != -1);
    if (h != -1) { CHECK_EQ_I(fd.time_create, 1000000100LL); CHECK_EQ_I(fd.time_access, 1000000200LL); _findclose(h); }
    DeleteFileA(F);
}
int main(void) { tc_begin(); DeleteFileA(F); TC_RUN(write_time); TC_RUN(create_access_time); return tc_end(); }
