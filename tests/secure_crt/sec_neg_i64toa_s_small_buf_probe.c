/* TC
 * id:    sec_neg_i64toa_s_small_buf_probe
 * area:  secure_crt
 * funcs:
 * procs: _i64toa_s _ui64toa_s _ltoa_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s family (buffer state after failure not clearly documented)
 * note:  PROBE: after ERANGE the buffer is an empty string (b[0]==0).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _ltoa_s, (long,char*,size_t,int));
TC_DECL_PROC(errno_t, _i64toa_s, (long long,char*,size_t,int));
TC_DECL_PROC(errno_t, _ui64toa_s, (unsigned long long,char*,size_t,int));
static volatile long long v_ll = 1234567890123LL;
static volatile unsigned long long v_ull = 1234567890123ULL;
static volatile long v_l = -123456;
TC_CASE(i64) { char b[5] = "zzzz"; tc_iph_install(); CHECK_EQ_I(tc_p__i64toa_s(v_ll, b, 5, 10), ERANGE); CHECK_EQ_I(b[0], 0); }
TC_CASE(ui64) { char b[5] = "zzzz"; tc_iph_install(); CHECK_EQ_I(tc_p__ui64toa_s(v_ull, b, 5, 10), ERANGE); CHECK_EQ_I(b[0], 0); }
TC_CASE(ltoa) { char b[4] = "zzz"; tc_iph_install(); CHECK_EQ_I(tc_p__ltoa_s(v_l, b, 4, 10), ERANGE); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_ltoa_s); TC_LOAD_PROC(_i64toa_s); TC_LOAD_PROC(_ui64toa_s); TC_RUN(i64); TC_RUN(ui64); TC_RUN(ltoa); return tc_end(); }
