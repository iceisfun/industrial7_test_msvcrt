/* TC
 * id:    scn_neg_sscanf_hh_bad_pre_ucrt_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Width specification" / scanf type-field size prefixes (z, j, t documented from VS2013/UCRT era)
 * note:  PROBES %zu / %jd / %td on pre-UCRT runtimes, where support is uncertain (may be ignored, rejected, or honoured).
 *        Asserts only: no crash, and no byte beyond the first 8 of a 16-byte sentinel buffer is modified.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile fmt;

static void probe(const char *f)
{
    unsigned char b[16]; int r, i; memset(b, 0xAA, sizeof b);
    tc_iph_count = 0; fmt = f; r = sscanf("5", fmt, b);
    (void)r;
    for (i = 8; i < 16; i++) CHECK_EQ_U(b[i], 0xAA);
}

TC_CASE(zu) { probe("%zu"); }
TC_CASE(jd) { probe("%jd"); }
TC_CASE(td) { probe("%td"); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(zu); TC_RUN(jd); TC_RUN(td); return tc_end(); }
