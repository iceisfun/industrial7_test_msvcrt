/* TC
 * id:    sec_makepath_s_basic
 * area:  secure_crt
 * funcs: _makepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _makepath_s, _wmakepath_s
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

static char *volatile v_null = NULL;
static volatile size_t v_sz = 64;
TC_CASE(all_parts) { char b[64]; CHECK_EQ_I(_makepath_s(b, v_sz, "C:", "\\dir\\sub\\", "name", ".ext"), 0); CHECK_STR(b, "C:\\dir\\sub\\name.ext"); }
TC_CASE(drive_letter_without_colon_gets_colon) { char b[64]; CHECK_EQ_I(_makepath_s(b, v_sz, "D", "\\x\\", "f", ".e"), 0); CHECK_STR(b, "D:\\x\\f.e"); }
TC_CASE(dir_without_trailing_slash_gets_slash) {
    char b[64];
    CHECK_EQ_I(_makepath_s(b, v_sz, "C:", "\\dir", "name", ".ext"), 0); CHECK_STR(b, "C:\\dir\\name.ext");
    CHECK_EQ_I(_makepath_s(b, v_sz, NULL, "a/b", "n", NULL), 0);
    CHECK(b[3] == '/' || b[3] == '\\'); CHECK_EQ_U(strlen(b), 4 + 1);
}
TC_CASE(ext_without_dot_gets_dot) { char b[64]; CHECK_EQ_I(_makepath_s(b, v_sz, NULL, NULL, "name", "txt"), 0); CHECK_STR(b, "name.txt"); }
TC_CASE(null_parts_skipped) {
    char b[64];
    CHECK_EQ_I(_makepath_s(b, v_sz, v_null, v_null, "name", v_null), 0); CHECK_STR(b, "name");
    CHECK_EQ_I(_makepath_s(b, v_sz, v_null, "\\d\\", v_null, ".x"), 0); CHECK_STR(b, "\\d\\.x");
}
TC_CASE(empty_parts) {
    char b[64];
    CHECK_EQ_I(_makepath_s(b, v_sz, "", "", "name", ""), 0); CHECK_STR(b, "name");
    CHECK_EQ_I(_makepath_s(b, v_sz, "", "", "", ""), 0); CHECK_STR(b, "");
}
TC_CASE(exact_fit) {
    char b[9]; memset(b, 'x', sizeof b);
    CHECK_EQ_I(_makepath_s(b, 9, NULL, NULL, "name", ".txt"), 0); CHECK_STR(b, "name.txt");
}
int main(void) { tc_begin(); TC_RUN(all_parts); TC_RUN(drive_letter_without_colon_gets_colon); TC_RUN(dir_without_trailing_slash_gets_slash);
    TC_RUN(ext_without_dot_gets_dot); TC_RUN(null_parts_skipped); TC_RUN(empty_parts); TC_RUN(exact_fit); return tc_end(); }
