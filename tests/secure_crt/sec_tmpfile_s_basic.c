/* TC
 * id:    sec_tmpfile_s_basic
 * area:  secure_crt
 * funcs:
 * procs: tmpfile_s fclose fwrite fread fseek
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpfile_s: creates a temporary file opened "w+b"; returns 0 on success
 * note:  Older CRTs create the file in the drive root and may fail with EACCES; ucrt may fail with ENOENT/EACCES if the temp dir is unwritable. Those are SKIPs.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *tmpfile_s_t)(void **);
typedef int (__cdecl *fclose_t)(void *);
typedef size_t (__cdecl *fwrite_t)(const void *, size_t, size_t, void *);
typedef size_t (__cdecl *fread_t)(void *, size_t, size_t, void *);
typedef int (__cdecl *fseek_t)(void *, long, int);
static tmpfile_s_t p_tmpfile_s; static fclose_t p_fclose; static fwrite_t p_fwrite; static fread_t p_fread; static fseek_t p_fseek;

TC_CASE(write_rewind_read)
{
    void *f = NULL; char b[16] = { 0 }; int r = p_tmpfile_s(&f);
    if (r == 2 || r == 13) TC_SKIP("tmpfile_s: ENOENT/EACCES");
    CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (!f) return;
    CHECK_EQ_U(p_fwrite("tmpdata", 1, 7, f), 7);
    CHECK_EQ_I(p_fseek(f, 0, 0), 0);
    CHECK_EQ_U(p_fread(b, 1, 15, f), 7);
    CHECK_STR(b, "tmpdata");
    p_fclose(f);
}
TC_CASE(pointer_set)
{
    void *f = (void *)(ULONG_PTR)1; int r = p_tmpfile_s(&f);
    if (r == 2 || r == 13) TC_SKIP("tmpfile_s: ENOENT/EACCES");
    CHECK_EQ_I(r, 0); CHECK(f != NULL); CHECK(f != (void *)(ULONG_PTR)1);
    if (f) p_fclose(f);
}
TC_CASE(fclose_ok)
{
    void *f = NULL; int r = p_tmpfile_s(&f);
    if (r == 2 || r == 13) TC_SKIP("tmpfile_s: ENOENT/EACCES");
    CHECK_EQ_I(r, 0);
    if (f) CHECK_EQ_I(p_fclose(f), 0);
}
int main(void)
{
    tc_begin();
    p_tmpfile_s = (tmpfile_s_t)tc_crt_proc("tmpfile_s"); p_fclose = (fclose_t)tc_crt_proc("fclose");
    p_fwrite = (fwrite_t)tc_crt_proc("fwrite"); p_fread = (fread_t)tc_crt_proc("fread"); p_fseek = (fseek_t)tc_crt_proc("fseek");
    CHECK(p_tmpfile_s && p_fclose && p_fwrite && p_fread && p_fseek);
    if (!(p_tmpfile_s && p_fclose && p_fwrite && p_fread && p_fseek)) return tc_end();
    TC_RUN(write_rewind_read); TC_RUN(pointer_set); TC_RUN(fclose_ok);
    return tc_end();
}
