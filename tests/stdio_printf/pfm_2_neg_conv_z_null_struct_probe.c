/* TC
 * id:    pfm_2_neg_conv_z_null_struct_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: sprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field Z; CRT convention for null string arguments
 * note:  PROBE: %Z with a NULL pointer, or with an ANSI_STRING whose Buffer is NULL, prints "(null)" like %s and does not call the invalid-parameter handler. sprintf_s counterpart included.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;

TC_CASE(null_ptr)   { char b[32]; TC_ANSI * volatile p = NULL; tc_iph_count = 0; sprintf(b, "[%Z]", p); CHECK_STR(b, "[(null)]"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(null_buf)   { char b[32]; TC_ANSI a = { 0, 0, NULL }; TC_ANSI * volatile p = &a; tc_iph_count = 0; sprintf(b, "[%Z]", p); CHECK_STR(b, "[(null)]"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(null_ptr_s) { if (!tc_p_sprintf_s) TC_SKIP("no sprintf_s"); char b[32]; TC_ANSI * volatile p = NULL; tc_iph_count = 0; tc_p_sprintf_s(b, sizeof b, "[%Z]", p); CHECK_STR(b, "[(null)]"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(sprintf_s);
    TC_RUN(null_ptr); TC_RUN(null_buf); TC_RUN(null_ptr_s);
    return tc_end();
}
