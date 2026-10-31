/* TC
 * id:    sec_strlwr_strupr_s_basic
 * area:  secure_crt
 * funcs: _strlwr_s _strupr_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strupr_s
 * note:  ASCII only, C locale
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(lower) { char b[16] = "HeLLo WORLD"; CHECK_EQ_I(_strlwr_s(b, sizeof b), 0); CHECK_STR(b, "hello world"); }
TC_CASE(upper) { char b[16] = "HeLLo world"; CHECK_EQ_I(_strupr_s(b, sizeof b), 0); CHECK_STR(b, "HELLO WORLD"); }
TC_CASE(non_alpha_unchanged) { char b[16] = "1-2_3 @[`{"; CHECK_EQ_I(_strlwr_s(b, sizeof b), 0); CHECK_STR(b, "1-2_3 @[`{"); CHECK_EQ_I(_strupr_s(b, sizeof b), 0); CHECK_STR(b, "1-2_3 @[`{"); }
TC_CASE(empty) { char b[4] = ""; CHECK_EQ_I(_strlwr_s(b, sizeof b), 0); CHECK_STR(b, ""); CHECK_EQ_I(_strupr_s(b, 1), 0); CHECK_STR(b, ""); }
TC_CASE(exact_fit) { char b[4] = "AbC"; CHECK_EQ_I(_strlwr_s(b, 4), 0); CHECK_STR(b, "abc"); CHECK_EQ_I(_strupr_s(b, 4), 0); CHECK_STR(b, "ABC"); }
int main(void) { tc_begin(); TC_RUN(lower); TC_RUN(upper); TC_RUN(non_alpha_unchanged); TC_RUN(empty); TC_RUN(exact_fit); return tc_end(); }
