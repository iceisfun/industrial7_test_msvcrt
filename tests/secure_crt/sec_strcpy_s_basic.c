/* TC
 * id:    sec_strcpy_s_basic
 * area:  secure_crt
 * funcs: strcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s, _mbscpy_s
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static void ini(void) { tc_iph_install(); tc_iph_count = 0; }

TC_CASE(copy_simple) { char d[16]; volatile const char *s = "hello"; ini(); memset(d, 'x', sizeof d); CHECK_EQ_I(strcpy_s(d, sizeof d, (const char *)s), 0); CHECK_STR(d, "hello"); }
TC_CASE(exact_fit) { char d[6]; volatile const char *s = "hello"; ini(); CHECK_EQ_I(strcpy_s(d, 6, (const char *)s), 0); CHECK_STR(d, "hello"); }
TC_CASE(empty_src) { char d[4]; volatile const char *s = ""; ini(); memset(d, 'x', sizeof d); CHECK_EQ_I(strcpy_s(d, sizeof d, (const char *)s), 0); CHECK_EQ_I(d[0], 0); }
TC_CASE(one_byte_buffer_empty_src) { char d[1]; volatile const char *s = ""; ini(); d[0] = 'x'; CHECK_EQ_I(strcpy_s(d, 1, (const char *)s), 0); CHECK_EQ_I(d[0], 0); }
TC_CASE(iph_not_called_on_success) { char d[8]; volatile const char *s = "abc"; ini(); strcpy_s(d, sizeof d, (const char *)s); strcpy_s(d, sizeof d, ""); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(copy_simple); TC_RUN(exact_fit); TC_RUN(empty_src); TC_RUN(one_byte_buffer_empty_src); TC_RUN(iph_not_called_on_success); return tc_end(); }
