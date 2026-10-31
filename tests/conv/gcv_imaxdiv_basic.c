/* TC
 * id:    gcv_imaxdiv_basic
 * area:  conv
 * funcs: imaxdiv
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C99 7.8.2.2; MS docs imaxdiv
 * note:  Not exported by msvcr110 and older.
 */
#include "tc.h"
#include <inttypes.h>
#include <stdint.h>

static volatile intmax_t n, d;

TC_CASE(signs)
{
    imaxdiv_t r;
    n = 7; d = 2; r = imaxdiv(n, d); CHECK_EQ_I(r.quot, 3); CHECK_EQ_I(r.rem, 1);
    n = -7; d = 2; r = imaxdiv(n, d); CHECK_EQ_I(r.quot, -3); CHECK_EQ_I(r.rem, -1);
    n = 7; d = -2; r = imaxdiv(n, d); CHECK_EQ_I(r.quot, -3); CHECK_EQ_I(r.rem, 1);
    n = -7; d = -2; r = imaxdiv(n, d); CHECK_EQ_I(r.quot, 3); CHECK_EQ_I(r.rem, -1);
}
TC_CASE(large) { imaxdiv_t r; n = INTMAX_MAX; d = 10; r = imaxdiv(n, d); CHECK_EQ_I(r.quot, 922337203685477580LL); CHECK_EQ_I(r.rem, 7); }
TC_CASE(zero_num) { imaxdiv_t r; n = 0; d = 5; r = imaxdiv(n, d); CHECK_EQ_I(r.quot, 0); CHECK_EQ_I(r.rem, 0); }
TC_CASE(identity)
{
    static const intmax_t nums[] = { 0, 1, -1, 99, -99, 123456789012345LL, -123456789012345LL, INTMAX_MAX, -INTMAX_MAX };
    static const intmax_t dens[] = { 1, -1, 3, -3, 1000, -7, 0x100000001LL, INTMAX_MAX };
    unsigned i, j;
    for (i = 0; i < sizeof nums / sizeof nums[0]; i++)
        for (j = 0; j < sizeof dens / sizeof dens[0]; j++) {
            imaxdiv_t r; n = nums[i]; d = dens[j]; r = imaxdiv(n, d);
            CHECK_EQ_I(r.quot * dens[j] + r.rem, nums[i]);
            CHECK(r.quot == nums[i] / dens[j]);
            CHECK(r.rem == nums[i] % dens[j]);
        }
}

int main(void) { tc_begin(); TC_RUN(signs); TC_RUN(large); TC_RUN(zero_num); TC_RUN(identity); return tc_end(); }
