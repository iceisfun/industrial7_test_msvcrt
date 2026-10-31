/* TC
 * id:    dir_neg_getdcwd_baddrive_probe
 * area:  dir
 * funcs: _getdcwd _wgetdcwd
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _getdcwd (drive range 0..26)
 * note:  Probes errno (EACCES expected) and iph count (1 expected) for out-of-range drive numbers -1 and 27. Separate cases probe an
 *        unused but syntactically valid drive letter: MS docs say an unavailable drive invokes the invalid parameter handler
 *        (CRT checks drive type), so NULL and handler count 1 are expected; unsure, hence probe.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_m1 = -1, v_27 = 27;

TC_CASE(drive27)
{
    char buf[MAX_PATH]; char *p;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0; p = _getdcwd(v_27, buf, MAX_PATH);
    CHECK(p == NULL); CHECK_EQ_I(errno, EACCES); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(drive_minus1)
{
    char buf[MAX_PATH]; char *p;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0; p = _getdcwd(v_m1, buf, MAX_PATH);
    CHECK(p == NULL); CHECK_EQ_I(errno, EACCES); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_drive27)
{
    wchar_t buf[MAX_PATH]; wchar_t *p;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0; p = _wgetdcwd(v_27, buf, MAX_PATH);
    CHECK(p == NULL); CHECK_EQ_I(errno, EACCES); CHECK_EQ_I(tc_iph_count, 1);
}
static int free_letter(void)
{
    unsigned long m = _getdrives(); int d;
    for (d = 1; d <= 26; d++) if (!(m & (1UL << (d - 1)))) return d;
    return 0;
}
TC_CASE(nonexistent_letter_null)
{
    char buf[MAX_PATH]; int d = free_letter();
    if (!d) TC_SKIP("all drive letters present");
    tc_iph_install();
    CHECK(_getdcwd(d, buf, MAX_PATH) == NULL);
}
TC_CASE(nonexistent_letter_iph)
{
    char buf[MAX_PATH]; int d = free_letter();
    if (!d) TC_SKIP("all drive letters present");
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; (void)_getdcwd(d, buf, MAX_PATH);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(drive27); TC_RUN(drive_minus1); TC_RUN(wide_drive27); TC_RUN(nonexistent_letter_null); TC_RUN(nonexistent_letter_iph);
    return tc_end();
}
