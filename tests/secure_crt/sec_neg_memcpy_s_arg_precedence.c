/* TC
 * id:    sec_neg_memcpy_s_arg_precedence
 * area:  secure_crt
 * funcs:
 * procs: memcpy_s memmove_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s / memmove_s (dest NULL -> EINVAL; count 0 is a no-op success)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, memcpy_s, (void*,size_t,const void*,size_t));
TC_DECL_PROC(errno_t, memmove_s, (void*,size_t,const void*,size_t));
static void *volatile v_null;
static const void *volatile v_cnull;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(cpy_null_dest_size0) { setup(); CHECK_EQ_I(tc_p_memcpy_s(v_null, 0, v_cnull, 4), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(cpy_both_null) { setup(); CHECK_EQ_I(tc_p_memcpy_s(v_null, 4, v_cnull, 4), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(cpy_count0) { char d[8] = "abcdefg"; setup(); CHECK_EQ_I(tc_p_memcpy_s(d, 8, "xyz", 0), 0); CHECK_STR(d, "abcdefg"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(mov_null_dest_size0) { setup(); CHECK_EQ_I(tc_p_memmove_s(v_null, 0, v_cnull, 4), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(mov_both_null) { setup(); CHECK_EQ_I(tc_p_memmove_s(v_null, 4, v_cnull, 4), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(mov_count0) { char d[8] = "abcdefg"; setup(); CHECK_EQ_I(tc_p_memmove_s(d, 8, "xyz", 0), 0); CHECK_STR(d, "abcdefg"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); TC_RUN(cpy_null_dest_size0); TC_RUN(cpy_both_null); TC_RUN(cpy_count0); TC_RUN(mov_null_dest_size0); TC_RUN(mov_both_null); TC_RUN(mov_count0); return tc_end(); }
