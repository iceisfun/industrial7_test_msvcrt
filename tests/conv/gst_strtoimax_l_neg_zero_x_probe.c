/* TC
 * id:    gst_strtoimax_l_neg_zero_x_probe
 * area:  conv
 * funcs: _strtoimax_l strtoumax _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.4 (subject sequence "0" in "0x" with no hex digit; endptr should point at the 'x')
 * note:  PROBES: for "0x" with base 16 (and base 0), value is 0 and endptr == nptr + 1. Some CRTs may leave endptr == nptr.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _strtoimax_l(const char *, char **, int, _locale_t);
extern uintmax_t __cdecl _strtoumax_l(const char *, char **, int, _locale_t);

static const char *volatile v = "0xZ";
static _locale_t loc;

TC_CASE(zero_x_base16) { char *e = NULL; CHECK_EQ_I(_strtoimax_l(v, &e, 16, loc), 0); CHECK_EQ_P(e, v + 1); }
TC_CASE(zero_x_base0)  { char *e = NULL; CHECK_EQ_U(strtoumax(v, &e, 0), 0); CHECK_EQ_P(e, v + 1); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(zero_x_base16); TC_RUN(zero_x_base0);
    _free_locale(loc);
    return tc_end();
}
