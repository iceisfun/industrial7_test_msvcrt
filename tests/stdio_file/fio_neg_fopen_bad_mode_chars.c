/* TC
 * id:    fio_neg_fopen_bad_mode_chars
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen: mode must begin with r, w or a
 * note:  Only modes whose first character is not r/w/a, or that are empty, are asserted here.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile fname = "tc_fio_neg_fopen_bad_mode_chars_a.tmp";
static const char * volatile m_x = "x";
static const char * volatile m_empty = "";
static const char * volatile m_qp = "q+";

static void bad(const char *mode)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(fname, mode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
}
TC_CASE(mode_x)     { bad(m_x); }
TC_CASE(mode_empty) { bad(m_empty); }
TC_CASE(mode_qplus) { bad(m_qp); }
TC_CASE(no_file_created) { CHECK_EQ_U(GetFileAttributesA(fname), INVALID_FILE_ATTRIBUTES); }
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    CHECK(tc_iph_install());
    TC_RUN(mode_x); TC_RUN(mode_empty); TC_RUN(mode_qplus); TC_RUN(no_file_created);
    DeleteFileA(fname);
    return tc_end();
}
