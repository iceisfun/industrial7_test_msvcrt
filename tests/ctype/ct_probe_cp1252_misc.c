/* TC
 * id:    ct_probe_cp1252_misc
 * area:  ctype
 * funcs: toupper islower isalpha setlocale
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs toupper, islower; cp1252 code page
 * note:  PROBE: cp1252 single-byte classification/case mapping via Windows NLS: 0xDF lower, no uppercase; 0xB5 lower; 0xFF -> 0x9F; ordinals 0xAA/0xBA alpha.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>
#undef toupper
#undef tolower
#undef islower
#undef isalpha

static volatile int v;
static int enter(void) { return setlocale(LC_ALL, "English_United States.1252") != NULL; }
static void leave(void) { setlocale(LC_ALL, "C"); }

TC_CASE(islower_0xDF_sharp_s_true) { if (!enter()) TC_SKIP("no cp1252"); v = 0xDF; CHECK(islower(v) != 0); leave(); }
TC_CASE(toupper_0xDF_unchanged) { if (!enter()) TC_SKIP("no cp1252"); v = 0xDF; CHECK_EQ_I(toupper(v), 0xDF); leave(); }
TC_CASE(islower_0xB5_micro_true) { if (!enter()) TC_SKIP("no cp1252"); v = 0xB5; CHECK(islower(v) != 0); leave(); }
TC_CASE(toupper_0xFF_is_0x9F) { if (!enter()) TC_SKIP("no cp1252"); v = 0xFF; CHECK_EQ_I(toupper(v), 0x9F); leave(); }
TC_CASE(isalpha_0xAA_ordinal_true) { if (!enter()) TC_SKIP("no cp1252"); v = 0xAA; CHECK(isalpha(v) != 0); leave(); }
TC_CASE(isalpha_0xBA_ordinal_true) { if (!enter()) TC_SKIP("no cp1252"); v = 0xBA; CHECK(isalpha(v) != 0); leave(); }

int main(void)
{
    tc_begin();
    TC_RUN(islower_0xDF_sharp_s_true); TC_RUN(toupper_0xDF_unchanged); TC_RUN(islower_0xB5_micro_true);
    TC_RUN(toupper_0xFF_is_0x9F); TC_RUN(isalpha_0xAA_ordinal_true); TC_RUN(isalpha_0xBA_ordinal_true);
    return tc_end();
}
