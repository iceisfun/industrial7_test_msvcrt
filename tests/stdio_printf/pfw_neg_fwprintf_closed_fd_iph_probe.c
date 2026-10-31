/* TC
 * id:    pfw_neg_fwprintf_closed_fd_iph_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf _close _fileno
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   UCRT/VC8+ _write validates the file descriptor with _VALIDATE_CLEAR_OSSERR_RETURN
 * note:  PROBE. Flushing a FILE whose descriptor was _close()d: probes that the invalid parameter handler is invoked exactly once
 *        (the CRT's _write validates fh and its FOPEN flag). Stricter than the documented EBADF/EOF result in pfw_neg_fwprintf_closed_fd.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>
#include <wchar.h>

static const char tmp[] = "tc_pfw_neg_fwprintf_closed_fd_iph_probe.tmp";

TC_CASE(handler_called)
{
    FILE *fp = fopen(tmp, "w");
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(_close(_fileno(fp)), 0);
    (void)fwprintf(fp, L"x");
    tc_iph_count = 0;
    CHECK_EQ_I(fflush(fp), EOF);
    CHECK_EQ_I(tc_iph_count, 1);
    (void)fclose(fp);
    remove(tmp);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(handler_called); return tc_end(); }
