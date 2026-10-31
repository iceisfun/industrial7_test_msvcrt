/* TC
 * id:    ucr_strtod_long_digits_probe
 * area:  ucrt_only
 * funcs: strtod
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.1.3 (beyond DECIMAL_DIG digits the standard allows either neighbouring value); UCRT is believed to round exactly
 * note:  Probes: strtod rounds exactly (ties-to-even, sticky digit) on a 55..855 digit input whose value is the exact midpoint
 *        1+2^-53 (-> 1.0), still the midpoint after 800 trailing zeros (-> 1.0), and just above it with a trailing '1'
 *        at digit 855 (-> 1+2^-52). The last one also probes that UCRT does not drop digits beyond its internal buffer.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v;
static char big[1024];
TC_CASE(long_digits) { char *e; int i, n = 0;
  const char *mid = "1.00000000000000011102230246251565404236316680908203125";
  while (mid[n]) { big[n] = mid[n]; n++; }
  v = big; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 0x3FF0000000000000ULL);
  for (i = 0; i < 800; i++) big[n++] = '0';
  big[n] = 0; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 0x3FF0000000000000ULL);
  big[n++] = '1'; big[n] = 0; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 0x3FF0000000000001ULL); CHECK_EQ_P(e, big + n); }

int main(void) { tc_begin(); TC_RUN(long_digits); return tc_end(); }
