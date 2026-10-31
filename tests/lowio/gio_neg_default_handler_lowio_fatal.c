/* TC
 * id:    gio_neg_default_handler_lowio_fatal
 * area:  lowio
 * funcs: _tell _filelength _eof _setmode _chsize
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation; _tell/_filelength/_eof/_setmode/_chsize with an invalid fd invoke the invalid parameter handler
 * note:  With the default handler the process terminates (exact status varies by CRT, so only "not 77 and not 0" is asserted).
 *        Reference: io_neg_default_handler_lseek_fatal.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

static int run_child(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    return ran;
}
TC_CASE(tell_child)       { run_child("tell"); }
TC_CASE(filelength_child) { run_child("filelength"); }
TC_CASE(eof_child)        { run_child("eof"); }
TC_CASE(setmode_child)    { run_child("setmode"); }
TC_CASE(chsize_child)     { run_child("chsize"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int bad = -1;
        if (tc_strequ(c, "tell")) _tell(bad);
        else if (tc_strequ(c, "filelength")) _filelength(bad);
        else if (tc_strequ(c, "eof")) _eof(bad);
        else if (tc_strequ(c, "setmode")) _setmode(bad, _O_BINARY);
        else if (tc_strequ(c, "chsize")) _chsize(bad, 0);
        return 77;
    }
    TC_RUN(tell_child); TC_RUN(filelength_child); TC_RUN(eof_child);
    TC_RUN(setmode_child); TC_RUN(chsize_child);
    return tc_end();
}
