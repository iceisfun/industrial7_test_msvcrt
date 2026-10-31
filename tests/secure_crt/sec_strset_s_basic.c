/* TC
 * id:    sec_strset_s_basic
 * area:  secure_crt
 * funcs: _strset_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(strset_fills_to_nul) { char b[8] = "abcde"; CHECK_EQ_I(_strset_s(b, sizeof b, 'z'), 0); CHECK_STR(b, "zzzzz"); CHECK_EQ_I(b[5], 0); }
TC_CASE(strset_empty) { char b[4] = ""; b[1] = 'k'; CHECK_EQ_I(_strset_s(b, sizeof b, 'z'), 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'k'); }
int main(void) { tc_begin(); TC_RUN(strset_fills_to_nul); TC_RUN(strset_empty); return tc_end(); }
