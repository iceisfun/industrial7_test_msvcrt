/* TC
 * id:    dat_iob_identities_win7
 * area:  data_exports
 * funcs: __iob_func _fileno
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fileno, stdin/stdout/stderr (file descriptors 0, 1, 2)
 * note:  With the mingw headers stdin is __acrt_iob_func(0), a static shim over __iob_func()[0], so iobfunc_stable is partly tautological;
 *        &__iob_func()[2] assumes sizeof(FILE) == 48 on x64 (true for the Microsoft FILE layout).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(three_distinct) { CHECK(stdin != stdout && stdout != stderr && stdin != stderr); CHECK(stdin && stdout && stderr); }
TC_CASE(fileno_0_1_2) { CHECK_EQ_I(_fileno(stdin), 0); CHECK_EQ_I(_fileno(stdout), 1); CHECK_EQ_I(_fileno(stderr), 2); }
TC_CASE(iobfunc_stable) { CHECK_EQ_P(__iob_func(), __iob_func()); CHECK_EQ_P(stdin, &__iob_func()[0]); CHECK_EQ_P(stderr, &__iob_func()[2]); }

int main(void) { tc_begin(); TC_RUN(three_distinct); TC_RUN(fileno_0_1_2); TC_RUN(iobfunc_stable); return tc_end(); }
