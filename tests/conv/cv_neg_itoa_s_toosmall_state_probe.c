/* TC
 * id:    cv_neg_itoa_s_toosmall_state_probe
 * area:  conv
 * funcs: _itoa_s _i64toa_s _ui64toa_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s (buffer[0] set to 0 on error)
 * note:  Probes the buffer after an ERANGE failure: buf[0]==0 and bytes at index >= size untouched (pre-filled 0xAA).
 *        Exact-fit-minus-one sizes. Whether bytes 1..size-1 are modified is NOT asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int v_min32 = (-2147483647 - 1);
static volatile long long v_min64 = (-9223372036854775807LL - 1);
static volatile unsigned long long v_max64 = 18446744073709551615ULL;

static void guard_ok(const unsigned char *b, size_t from, size_t n) { size_t i; for (i = from; i < n; i++) if (b[i] != 0xAA) { CHECK_EQ_U(b[i], 0xAA); break; } }

TC_CASE(itoa_s_state)
{
    unsigned char b[40]; size_t i; tc_iph_install();
    for (i = 0; i < sizeof b; i++) b[i] = 0xAA;
    CHECK_EQ_I(_itoa_s(v_min32, (char *)b, 11, 10), ERANGE);
    CHECK_EQ_U(b[0], 0); guard_ok(b, 11, sizeof b);
}
TC_CASE(i64toa_s_state)
{
    unsigned char b[40]; size_t i; tc_iph_install();
    for (i = 0; i < sizeof b; i++) b[i] = 0xAA;
    CHECK_EQ_I(_i64toa_s(v_min64, (char *)b, 20, 10), ERANGE);
    CHECK_EQ_U(b[0], 0); guard_ok(b, 20, sizeof b);
}
TC_CASE(ui64toa_s_state)
{
    unsigned char b[40]; size_t i; tc_iph_install();
    for (i = 0; i < sizeof b; i++) b[i] = 0xAA;
    CHECK_EQ_I(_ui64toa_s(v_max64, (char *)b, 20, 10), ERANGE);
    CHECK_EQ_U(b[0], 0); guard_ok(b, 20, sizeof b);
}
int main(void) { tc_begin(); TC_RUN(itoa_s_state); TC_RUN(i64toa_s_state); TC_RUN(ui64toa_s_state); return tc_end(); }
