/* TC
 * id:    pfm_3_conv_p_prec_star_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field p (pointer printed as hex digits, upper case, zero padded to the pointer width); precision specification
 * note:  PROBE: precision (including star and .0) is ignored for %p: the output always equals the plain %p output (2*sizeof(void*) upper-case
 *        hex digits, zero padded), and the star argument is consumed. NULL prints as all zeros.
 */
#include "tc.h"
#include <stdio.h>
#include <stdint.h>

static void * volatile vp = (void *)(uintptr_t)0x1234ABCD;
static void * volatile vnull = 0;
static const char * volatile vf;
static int slen(const char *s) { int n = 0; while (s[n]) n++; return n; }

TC_CASE(plain_form) {
    char b[48]; sprintf(b, "%p", vp);
    CHECK_EQ_I(slen(b), (int)(2 * sizeof(void *)));
    CHECK(b[slen(b) - 1] == 'D'); CHECK(b[slen(b) - 4] == 'A');
}
TC_CASE(star4)  { char a[48], b[48]; sprintf(a, "%p", vp); vf = "%.*p|%d"; sprintf(b, vf, 4, vp, 9);  CHECK_EQ_I(slen(b), slen(a) + 2); b[slen(a)] = 0; CHECK_STR(b, a); }
TC_CASE(star16) { char a[48], b[48]; sprintf(a, "%p", vp); vf = "%.*p|%d"; sprintf(b, vf, 16, vp, 9); CHECK_EQ_I(slen(b), slen(a) + 2); b[slen(a)] = 0; CHECK_STR(b, a); }
TC_CASE(star_vs_zero_star) { char a[48], b[48]; vf = "%.*p"; sprintf(a, vf, 20, vp); vf = "%0*p"; sprintf(b, vf, 20, vp);
    CHECK_EQ_I(slen(a), (int)(2 * sizeof(void *))); CHECK_EQ_I(slen(b), 20); }
TC_CASE(dot0_null) { char a[48], b[48]; sprintf(a, "%p", vnull); vf = "%.0p"; sprintf(b, vf, vnull); CHECK_STR(b, a); CHECK_EQ_I(slen(b), (int)(2 * sizeof(void *))); }
TC_CASE(dot0_ptr)  { char a[48], b[48]; sprintf(a, "%p", vp); vf = "%.0p"; sprintf(b, vf, vp); CHECK_STR(b, a); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(plain_form); TC_RUN(star4); TC_RUN(star16); TC_RUN(star_vs_zero_star); TC_RUN(dot0_null); TC_RUN(dot0_ptr);
    return tc_end();
}
