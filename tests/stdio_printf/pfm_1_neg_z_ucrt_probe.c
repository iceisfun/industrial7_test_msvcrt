/* TC
 * id:    pfm_1_neg_z_ucrt_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf type field (ucrt does not list Z)
 * note:  Observed Win10 22H2 ucrtbase: %Z is NOT rejected; in the NARROW printf it consumes a UNICODE_STRING (Length in bytes, wide Buffer): "[%Z]" with {6,14,L"abcdef"} -> "[abc]" (5), no EINVAL, no handler call. (A narrow ANSI_STRING needs %hZ; handing one to plain %Z reads it as wide chars and fails -1/EILSEQ. See pfs_probe_str_wz_unicode_string for the wide printf.)
 *        (Old expectation: -1/EINVAL/handler; baseline raw: r == -1 got=0 for a zeroed 'string'.) Deviates from the printf docs type-field list, which has no Z.
 *        Case name kept for ledger continuity (Z_narrow_unicode_string now asserts acceptance).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

typedef struct { unsigned short Length, MaximumLength; wchar_t *Buffer; } TC_UNICODE_STRING;
static wchar_t wtxt[] = L"abcdef";

TC_CASE(Z_narrow_unicode_string) {
    char b[32]; int r; TC_UNICODE_STRING us;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    us.Length = 3 * sizeof(wchar_t); us.MaximumLength = 14; us.Buffer = wtxt;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, "[%Z]", (void *)&us);
    CHECK_EQ_I(r, 5); CHECK_STR(b, "[abc]"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(Z_narrow_unicode_string); return tc_end(); }
