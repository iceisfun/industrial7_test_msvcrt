/* TC
 * id:    dir_neg_findfirst_variants_nomatch
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst32 _findfirst64 _findfirst64i32 _findnext32 _findnext64 _findnext64i32 _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _findfirst, _findnext (-1 + ENOENT when spec cannot be matched; NULL filespec/fileinfo -> invalid parameter handler, EINVAL)
 * note:  Every struct variant is exercised through a name table and resolved at run time. _findnext(-1,..) only asserts the -1
 *        return, errno is not asserted.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

typedef intptr_t (__cdecl *ff_t)(const char *, void *);
typedef int (__cdecl *fn_t)(intptr_t, void *);
static const char *const ffn[] = { "_findfirst32", "_findfirst64", "_findfirst64i32" };
static const char *const fnn[] = { "_findnext32", "_findnext64", "_findnext64i32" };
#define NV 3
static ff_t pff[NV];
static fn_t pfn[NV];
static union { long long al; char b[1024]; } fdb;
static const char *volatile v_nomatch = "tc_dir_neg_ffv_nodir_tmp\\nonexistent*.zz";
static const char *volatile v_nodir = "tc_dir_neg_ffv_nomatch.zz";
static const char *volatile v_file = "tc_dir_neg_ffv_one.tmp";
static const char *volatile v_null_spec;
static void *volatile v_null_fd;

static void mkfile(const char *p)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}

TC_CASE(nomatch_all_variants)
{
    int i; intptr_t h;
    for (i = 0; i < NV; i++) {
        if (!pff[i]) continue;
        errno = 0; h = pff[i](v_nodir, &fdb);
        CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
        if (h != -1) _findclose(h);
        errno = 0; h = pff[i](v_nomatch, &fdb);
        CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
        if (h != -1) _findclose(h);
    }
}
TC_CASE(null_spec_all_variants)
{
    int i; intptr_t h;
    tc_iph_install();
    for (i = 0; i < NV; i++) {
        if (!pff[i]) continue;
        tc_iph_count = 0; errno = 0; h = pff[i](v_null_spec, &fdb);
        CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        if (h != -1) _findclose(h);
    }
}
TC_CASE(null_result_all_variants)
{
    int i; intptr_t h;
    tc_iph_install();
    for (i = 0; i < NV; i++) {
        if (!pff[i]) continue;
        tc_iph_count = 0; errno = 0; h = pff[i]("*", v_null_fd);
        CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        if (h != -1) _findclose(h);
    }
}
TC_CASE(findnext_bad_handle)
{
    int i; volatile intptr_t bad = -1;
    tc_iph_install();
    for (i = 0; i < NV; i++) {
        if (!pfn[i]) continue;
        CHECK_EQ_I(pfn[i](bad, &fdb), -1);
    }
}
TC_CASE(success_handle_closed)
{
    int i; intptr_t h;
    mkfile(v_file);
    for (i = 0; i < NV; i++) {
        if (!pff[i]) continue;
        h = pff[i](v_file, &fdb);
        CHECK(h != -1);
        if (h != -1) CHECK_EQ_I(_findclose(h), 0);
    }
    DeleteFileA(v_file);
}

int main(void)
{
    int i;
    tc_begin();
    for (i = 0; i < NV; i++) { pff[i] = (ff_t)tc_crt_proc(ffn[i]); pfn[i] = (fn_t)tc_crt_proc(fnn[i]); }
    TC_RUN(nomatch_all_variants); TC_RUN(null_spec_all_variants);
    TC_RUN(findnext_bad_handle); TC_RUN(success_handle_closed); TC_RUN(null_result_all_variants);
    return tc_end();
}
