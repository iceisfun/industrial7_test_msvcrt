/* TC
 * id:    io_fileno_roundtrip
 * area:  lowio
 * funcs: _fileno
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _fileno; stdin/stdout/stderr are descriptors 0/1/2
 * note:  Cross-area with stdio; only the standard descriptor numbers are checked. Assumes a console-subsystem exe whose
 *        stdout/stderr are associated with streams (docs: _fileno returns -2 for a GUI app with no stdout/stderr).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>

TC_CASE(std_streams)
{
    CHECK_EQ_I(_fileno(stdin), 0);
    CHECK_EQ_I(_fileno(stdout), 1);
    CHECK_EQ_I(_fileno(stderr), 2);
}
int main(void) { tc_begin(); TC_RUN(std_streams); return tc_end(); }
