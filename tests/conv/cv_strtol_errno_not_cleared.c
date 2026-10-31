/* TC
 * id:    cv_strtol_errno_not_cleared
 * area:  conv
 * funcs: strtol strtoul
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.5p3 (no library function sets errno to zero)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(double, strtod, (const char *, char **));

TC_CASE(strtol_keeps) { volatile const char *s = "1"; char *e; errno = ERANGE; CHECK_EQ_I(strtol((const char *)s, &e, 10), 1); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(strtod_keeps) { volatile const char *s = "1.5"; char *e; errno = ERANGE; CHECK(tc_p_strtod((const char *)s, &e) == 1.5); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(strtoul_keeps) { volatile const char *s = "7"; char *e; errno = ERANGE; CHECK_EQ_U(strtoul((const char *)s, &e, 10), 7); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_LOAD_PROC(strtod); CHECK(tc_p_strtod); if (!tc_p_strtod) return 1; TC_RUN(strtol_keeps); TC_RUN(strtod_keeps); TC_RUN(strtoul_keeps); return tc_end(); }
