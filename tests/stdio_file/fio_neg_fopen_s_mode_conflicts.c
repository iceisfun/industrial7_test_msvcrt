/* TC
 * id:    fio_neg_fopen_s_mode_conflicts
 * area:  stdio_file
 * kind:  negative
 * funcs: fclose
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fopen_s: on failure *pFile is set to NULL and an error code returned; "Parameter validation"
 * note:  Mode strings with an invalid first character (empty, 'z'): EINVAL return, *pFile NULLed, handler once, no file
 *        created. Conflicting-suffix modes (rr, rbt) are NOT asserted here: they may be silently accepted by old CRTs.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
#define F "tc_fio_neg_fopen_s_mode_conflicts_" V ".tmp"
static const char * volatile fname = F;
TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static void bad(const char *mode)
{
    FILE *f = (FILE *)1; int n0 = tc_iph_count, r;
    if (!tc_p_fopen_s) return;
    r = tc_p_fopen_s(&f, fname, mode);
    CHECK_EQ_I(r, EINVAL);
    CHECK(f == NULL);
    if (f && f != (FILE *)1) fclose(f);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
static const char * volatile m_z = "z", * volatile m_empty = "";
TC_CASE(exported) { CHECK(tc_p_fopen_s != NULL); }
TC_CASE(bad_letter) { bad(m_z); }
TC_CASE(empty) { bad(m_empty); }
TC_CASE(file_ptr_preset_nulled)
{
    FILE *f = (FILE *)1; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, fname, m_z);
    CHECK_EQ_I(r, EINVAL); CHECK(f == NULL);
    CHECK(!exists(fname));
}
int main(void)
{
    tc_begin(); DeleteFileA(fname); TC_LOAD_PROC(fopen_s);
    CHECK(tc_iph_install());
    TC_RUN(exported); TC_RUN(bad_letter); TC_RUN(empty); TC_RUN(file_ptr_preset_nulled);
    DeleteFileA(fname);
    return tc_end();
}
