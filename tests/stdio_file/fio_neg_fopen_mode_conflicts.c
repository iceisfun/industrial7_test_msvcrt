/* TC
 * id:    fio_neg_fopen_mode_conflicts
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fopen (mode string: first character must be r, w or a); "Parameter validation"
 * note:  Only mode strings whose FIRST character is not r/w/a (empty, 'z', '1') are asserted: EINVAL + handler once, no file
 *        created. Conflicting/duplicate suffix letters (rbt, r+b+, rr ...) are probed separately in
 *        fio_neg_probe_fopen_mode_letter_conflicts because the CRT may silently stop parsing there.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define V TC_S1(TC_CRT_VER)
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
#define F "tc_fio_neg_fopen_modeconf_" V ".tmp"
static const char * volatile fname = F;
static void bad(const char *mode)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(fname, mode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
}
static const char * volatile m_empty = "", * volatile m_z = "z", * volatile m_digit = "1";
TC_CASE(empty_mode)  { bad(m_empty); }
TC_CASE(bad_first_letter) { bad(m_z); bad(m_digit); }
TC_CASE(no_file_left)
{
    DeleteFileA(fname); bad(m_z);
    CHECK(!exists(fname));
}
int main(void)
{
    tc_begin(); DeleteFileA(fname);
    CHECK(tc_iph_install());
    TC_RUN(empty_mode); TC_RUN(bad_first_letter); TC_RUN(no_file_left);
    DeleteFileA(fname);
    return tc_end();
}
