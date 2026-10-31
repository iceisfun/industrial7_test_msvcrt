/* TC
 * id:    dir_neg_wfindfirst_nomatch
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _wfindfirst32 _wfindfirst64 _wfindfirst64i32 _wfindnext64i32 _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _wfindfirst, _wfindnext (-1 and ENOENT when no match / enumeration exhausted; NULL fileinfo -> EINVAL via invalid parameter handler)
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

typedef intptr_t (__cdecl *wff_t)(const wchar_t *, void *);
typedef int (__cdecl *wfn_t)(intptr_t, void *);
static wff_t p_f32, p_f64, p_f64i32;
static wfn_t p_n64i32;
static union { long long al; char b[1024]; } fdb;
static const wchar_t *volatile v_nomatch = L"nonexistent*.zz";
static const wchar_t *volatile v_nodir = L"nodir\\*";
static const wchar_t *volatile v_one = L"tc_dir_neg_wff_one.tmp";
static void *volatile v_null_fd;

TC_CASE(nomatch)
{
    intptr_t h; wff_t f[3]; int i;
    f[0] = p_f32; f[1] = p_f64; f[2] = p_f64i32;
    for (i = 0; i < 3; i++) {
        if (!f[i]) continue;
        errno = 0; h = f[i](v_nomatch, &fdb);
        CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
        if (h != -1) _findclose(h);
    }
}
TC_CASE(missing_dir)
{
    intptr_t h; wff_t f[3]; int i;
    f[0] = p_f32; f[1] = p_f64; f[2] = p_f64i32;
    for (i = 0; i < 3; i++) {
        if (!f[i]) continue;
        errno = 0; h = f[i](v_nodir, &fdb);
        CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
        if (h != -1) _findclose(h);
    }
}
TC_CASE(exhaustion)
{
    intptr_t h; int r; HANDLE fh;
    if (!p_f64i32 || !p_n64i32) TC_SKIP("not exported");
    fh = CreateFileW(v_one, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fh != INVALID_HANDLE_VALUE) CloseHandle(fh);
    h = p_f64i32(v_one, &fdb);
    CHECK(h != -1);
    if (h != -1) {
        errno = 0; r = p_n64i32(h, &fdb);
        CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
        CHECK_EQ_I(_findclose(h), 0);
    }
    DeleteFileW(v_one);
}
TC_CASE(null_result)
{
    intptr_t h;
    if (!p_f64i32) TC_SKIP("not exported");
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    h = p_f64i32(L"*", v_null_fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (h != -1) _findclose(h);
}

int main(void)
{
    tc_begin();
    p_f32 = (wff_t)tc_crt_proc("_wfindfirst32"); p_f64 = (wff_t)tc_crt_proc("_wfindfirst64");
    p_f64i32 = (wff_t)tc_crt_proc("_wfindfirst64i32"); p_n64i32 = (wfn_t)tc_crt_proc("_wfindnext64i32");
    TC_RUN(nomatch); TC_RUN(missing_dir); TC_RUN(exhaustion); TC_RUN(null_result);
    return tc_end();
}
