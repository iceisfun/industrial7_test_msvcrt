/* TC
 * id:    ct_eof_all_classes_zero
 * area:  ctype
 * funcs: isalpha isdigit isspace ispunct isprint iscntrl isxdigit isalnum isgraph isupper islower toupper tolower
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.4.1, 7.4.2; MS docs isalpha (EOF allowed)
 * note:  IPH check only where _set_invalid_parameter_handler exists (>=80).
 */
#include "tc.h"
#include <ctype.h>
#include <stdio.h>

static volatile int v = EOF;

TC_CASE(all_is_funcs_zero_for_EOF)
{
    CHECK_EQ_I((isalpha)(v), 0); CHECK_EQ_I((isdigit)(v), 0); CHECK_EQ_I((isspace)(v), 0);
    CHECK_EQ_I((ispunct)(v), 0); CHECK_EQ_I((isprint)(v), 0); CHECK_EQ_I((iscntrl)(v), 0);
    CHECK_EQ_I((isxdigit)(v), 0); CHECK_EQ_I((isalnum)(v), 0); CHECK_EQ_I((isgraph)(v), 0);
    CHECK_EQ_I((isupper)(v), 0); CHECK_EQ_I((islower)(v), 0);
}
TC_CASE(toupper_EOF_is_EOF) { CHECK_EQ_I((toupper)(v), EOF); }
TC_CASE(tolower_EOF_is_EOF) { CHECK_EQ_I((tolower)(v), EOF); }
TC_CASE(no_crash_no_iph)
{
    int n0, got = 0;
    if (!TC_AT_LEAST(80)) TC_SKIP("no invalid parameter handler API");
    got = tc_iph_install();
    if (!got) TC_SKIP("handler not installable");
    n0 = tc_iph_count;
    (void)(isalpha)(v); (void)(isdigit)(v); (void)(isspace)(v); (void)(ispunct)(v); (void)(isprint)(v);
    (void)(iscntrl)(v); (void)(isxdigit)(v); (void)(isalnum)(v); (void)(isgraph)(v); (void)(isupper)(v);
    (void)(islower)(v); (void)(toupper)(v); (void)(tolower)(v);
    CHECK_EQ_I(tc_iph_count, n0);
}

int main(void) { tc_begin(); TC_RUN(all_is_funcs_zero_for_EOF); TC_RUN(toupper_EOF_is_EOF); TC_RUN(tolower_EOF_is_EOF); TC_RUN(no_crash_no_iph); return tc_end(); }
