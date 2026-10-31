/* TC
 * id:    pfm_1_width_num_z_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf format specification (Z: ANSI_STRING / UNICODE_STRING structure pointer)
 * note:  PROBES that %Z prints the Buffer of an ANSI_STRING (Length bytes), honours width, and that a NULL pointer
 *        prints "(null)". Most uncertain: NULL handling and width support.
 *        UCRT may instead treat %Z as an invalid format (invalid-parameter handler, -1); a handler is installed so
 *        the process survives and each case reports independently.
 */
#include "tc.h"
#include <stdio.h>

typedef struct { unsigned short Length, MaximumLength; char *Buffer; } TC_ANSI_STRING;
static char buf3[] = "abcdef";
static TC_ANSI_STRING as3 = { 3, 6, buf3 };
static TC_ANSI_STRING * volatile pas = &as3, * volatile pnull = NULL;

TC_CASE(plain) { char b[64]; sprintf(b, "[%Z]", pas); CHECK_STR(b, "[abc]"); }
TC_CASE(width) { char b[64]; sprintf(b, "[%6Z]", pas); CHECK_STR(b, "[   abc]"); }
TC_CASE(left) { char b[64]; sprintf(b, "[%-6Z]", pas); CHECK_STR(b, "[abc   ]"); }
TC_CASE(null_ptr) { char b[64]; sprintf(b, "[%Z]", pnull); CHECK_STR(b, "[(null)]"); }
int main(void) { tc_begin(); tc_iph_install(); /* an unsupported %Z must not kill the process */ TC_RUN(plain); TC_RUN(width); TC_RUN(left); TC_RUN(null_ptr); return tc_end(); }
