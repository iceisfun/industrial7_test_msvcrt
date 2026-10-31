/* TC
 * id:    str_stricmp_highbit_unsigned_probe
 * area:  string
 * funcs: _stricmp
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none
 * note:  PROBE: in the C locale _stricmp compares bytes >=0x80 as unsigned (greater than any ASCII byte).
 */
#include "tc.h"
#include <string.h>

static volatile const char *pa, *pb;
static int ci(const char *a, const char *b) { pa = a; pb = b; return _stricmp((const char *)pa, (const char *)pb); }

TC_CASE(e9_vs_e) { CHECK(ci("\xe9", "e") > 0); }
TC_CASE(80_vs_7f) { CHECK(ci("\x80", "\x7f") > 0); }
int main(void) { tc_begin(); TC_RUN(e9_vs_e); TC_RUN(80_vs_7f); return tc_end(); }
