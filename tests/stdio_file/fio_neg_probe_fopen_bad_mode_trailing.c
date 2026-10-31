/* TC
 * id:    fio_neg_probe_fopen_bad_mode_trailing
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * procs: 
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (mode flags); C11 7.21.5.3p3
 * note:  Probes: 'rw' and 'wz' (valid first char, bad later char) return NULL with EINVAL, and 'wz' creates no file. Standard-invalid; the exact MS handling is what is measured.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile fname = "tc_fio_neg_probe_fopen_bad_mode_trailing_a.tmp";
static const char * volatile m_rw = "rw";
static const char * volatile m_wz = "wz";

TC_CASE(mode_rw)
{
    FILE *f; errno = 0;
    f = fopen(fname, m_rw);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    if (f) fclose(f);
}
TC_CASE(mode_wz)
{
    FILE *f; errno = 0;
    f = fopen(fname, m_wz);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    if (f) fclose(f);
    CHECK_EQ_U(GetFileAttributesA(fname), INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fname);
    tc_iph_install();
    TC_RUN(mode_rw); TC_RUN(mode_wz);
    DeleteFileA(fname);
    return tc_end();
}
