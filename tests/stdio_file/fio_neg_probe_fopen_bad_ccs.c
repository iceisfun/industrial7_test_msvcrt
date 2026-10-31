/* TC
 * id:    fio_neg_probe_fopen_bad_ccs
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * procs: 
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen: ccs flag accepts UNICODE, UTF-8, UTF-16LE
 * note:  Probes: unrecognised ccs= values are rejected with NULL/EINVAL via the invalid parameter handler. An existing file is used so a failure is not ENOENT.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile fname = "tc_fio_neg_probe_fopen_bad_ccs_a.tmp";
static const char * volatile m_foo = "r, ccs=FOO";
static const char * volatile m_none = "r, ccs=";
static const char * volatile m_utf9 = "r, ccs=UTF-9";

static void bad(const char *mode)
{
    FILE *f; int n0 = tc_iph_count; errno = 0;
    f = fopen(fname, mode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    if (f) fclose(f);
}
TC_CASE(ccs_foo)  { bad(m_foo); }
TC_CASE(ccs_none) { bad(m_none); }
TC_CASE(ccs_utf9) { bad(m_utf9); }
int main(void)
{
    HANDLE h;
    tc_begin();
    h = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { tc_write("TC FAIL setup\n"); return 2; }
    CloseHandle(h);
    tc_iph_install();
    TC_RUN(ccs_foo); TC_RUN(ccs_none); TC_RUN(ccs_utf9);
    DeleteFileA(fname);
    return tc_end();
}
