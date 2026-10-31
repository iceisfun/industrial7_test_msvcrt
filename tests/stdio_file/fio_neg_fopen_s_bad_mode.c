/* TC
 * id:    fio_neg_fopen_s_bad_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s: invalid mode -> EINVAL
 * note:  Modes with an invalid first character and the empty mode.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));
static const char * volatile fname = "tc_fio_neg_fopen_s_bad_mode_a.tmp";
static const char * volatile m_z = "z";
static const char * volatile m_empty = "";

static void bad(const char *mode)
{
    FILE *f = (FILE *)(ULONG_PTR)0x1000; int r, n0 = tc_iph_count;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, fname, mode);
    CHECK_EQ_I(r, EINVAL);
    CHECK(f == NULL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
}
TC_CASE(mode_z) { bad(m_z); }
TC_CASE(mode_empty) { bad(m_empty); }
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    TC_LOAD_PROC(fopen_s);
    CHECK(tc_p_fopen_s != NULL);
    CHECK(tc_iph_install());
    TC_RUN(mode_z); TC_RUN(mode_empty);
    DeleteFileA(fname);
    return tc_end();
}
