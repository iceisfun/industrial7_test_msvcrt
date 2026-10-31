/* TC
 * id:    ct_cp1252_case_mapping
 * area:  ctype
 * funcs: toupper tolower setlocale
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs toupper, tolower, setlocale
 * note:  Excludes 0xFF/0xB5/0xDF. TC_SKIP if locale missing.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static int have;
TC_CASE(setlocale_ok_or_skip) { have = setlocale(LC_ALL, "English_United States.1252") != NULL; if (!have) TC_SKIP("locale unavailable"); }
TC_CASE(toupper_e_acute_0xE9_to_0xC9) { volatile int c = 0xE9; if (!have) TC_SKIP("no locale"); CHECK_EQ_I(toupper(c), 0xC9); }
TC_CASE(tolower_0xC9_to_0xE9) { volatile int c = 0xC9; if (!have) TC_SKIP("no locale"); CHECK_EQ_I(tolower(c), 0xE9); CHECK_EQ_I(tolower(0xE9), 0xE9); CHECK_EQ_I(toupper(0xC9), 0xC9); }
TC_CASE(a_ring_0xE5_0xC5) { volatile int c = 0xE5; if (!have) TC_SKIP("no locale"); CHECK_EQ_I(toupper(c), 0xC5); c = 0xC5; CHECK_EQ_I(tolower(c), 0xE5); }
TC_CASE(non_letters_0xD7_0xF7_unchanged)
{
    volatile int c = 0xD7;
    if (!have) TC_SKIP("no locale");
    CHECK_EQ_I(toupper(c), 0xD7); CHECK_EQ_I(tolower(c), 0xD7);
    c = 0xF7; CHECK_EQ_I(toupper(c), 0xF7); CHECK_EQ_I(tolower(c), 0xF7);
}
TC_CASE(restore_C_locale)
{
    volatile int c = 0xE9;
    CHECK(setlocale(LC_ALL, "C") != NULL);
    CHECK_EQ_I(toupper(c), 0xE9);
}
int main(void) { tc_begin(); TC_RUN(setlocale_ok_or_skip); TC_RUN(toupper_e_acute_0xE9_to_0xC9); TC_RUN(tolower_0xC9_to_0xE9); TC_RUN(a_ring_0xE5_0xC5); TC_RUN(non_letters_0xD7_0xF7_unchanged); TC_RUN(restore_C_locale); return tc_end(); }
