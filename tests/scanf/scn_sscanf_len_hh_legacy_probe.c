/* TC
 * id:    scn_sscanf_len_hh_legacy_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf width/size fields (hh not documented pre-UCRT)
 * note:  PROBE: what pre-UCRT does with %hhd (hh unsupported?). Installs the invalid-parameter handler (msvcr80+ may reject the format). Deliberately weak: only asserts no crash, return<=1 and that bytes 4..7 next to the target int are untouched (a 2- or 4-byte store at most).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(hh_no_crash)
{
    union { int a[2]; unsigned char buf[8]; } u; unsigned char *buf = u.buf; int n; int i;
    for (i = 0; i < 8; i++) buf[i] = 0x5A;
    /* msvcr80+ may treat the unknown 'hh' size as an invalid format and call the invalid-parameter handler (fatal by default) */
    tc_iph_install();
    v = "12";
    n = sscanf((const char *)v, "%hhd", &u.a[0]);
    CHECK(n <= 1);
    for (i = 4; i < 8; i++) CHECK_EQ_U(buf[i], 0x5A);
}
int main(void) { tc_begin(); TC_RUN(hh_no_crash); return tc_end(); }
