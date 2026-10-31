/* TC
 * id:    ct_probe_iswspace_nbsp
 * area:  ctype
 * funcs: iswspace
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs iswspace
 * note:  PROBE: iswspace(U+00A0) and iswspace(U+0085) nonzero in the C locale (Latin-1 classified via the Unicode/NLS table). U+0085 is the less certain. (isspace(0xA0)==0 in C locale is covered by the narrow high-byte tests.)
 */
#include "tc.h"
#include <wctype.h>
#undef iswspace

static volatile wint_t w;

TC_CASE(iswspace_0x00A0_nonzero) { w = 0xA0; CHECK(iswspace(w) != 0); }
TC_CASE(iswspace_0x0085_nonzero) { w = 0x85; CHECK(iswspace(w) != 0); }

int main(void)
{
    tc_begin();
    TC_RUN(iswspace_0x00A0_nonzero); TC_RUN(iswspace_0x0085_nonzero);
    return tc_end();
}
