/* TC
 * id:    cv_strtol_l_basic
 * area:  conv
 * funcs: _strtol_l _atoi_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs strtol, wcstol, _strtol_l; atoi, _atoi_l
 * note:  Passing a NULL locale_t is treated as "use the current locale"; widely relied upon but not spelled out in MS docs (hence observed).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

TC_CASE(dec) { volatile const char *s = "12xy"; char *e; CHECK_EQ_I(_strtol_l((const char *)s, &e, 10, NULL), 12); CHECK_EQ_P(e, s + 2); }
TC_CASE(atoi_neg) { volatile const char *s = "-5"; CHECK_EQ_I(_atoi_l((const char *)s, NULL), -5); }
TC_CASE(hex) { volatile const char *s = "0x1F"; char *e; CHECK_EQ_I(_strtol_l((const char *)s, &e, 16, NULL), 31); CHECK_EQ_I(_strtol_l((const char *)s, &e, 0, NULL), 31); }
TC_CASE(overflow)
{
    volatile const char *s = "2147483648"; volatile const char *n = "-2147483649"; char *e;
    errno = 0; CHECK_EQ_I(_strtol_l((const char *)s, &e, 10, NULL), LONG_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0; CHECK_EQ_I(_strtol_l((const char *)n, &e, 10, NULL), LONG_MIN); CHECK_EQ_I(errno, ERANGE);
}

int main(void) { tc_begin(); TC_RUN(dec); TC_RUN(atoi_neg); TC_RUN(hex); TC_RUN(overflow); return tc_end(); }
