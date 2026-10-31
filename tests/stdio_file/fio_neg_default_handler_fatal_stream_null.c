/* TC
 * id:    fio_neg_default_handler_fatal_stream_null
 * area:  stdio_file
 * kind:  negative
 * funcs: fread ftell rewind fgetpos feof ferror clearerr
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation"; docs for fread/ftell/rewind/fgetpos/feof/ferror/clearerr (invalid-parameter handler is invoked for NULL stream)
 * note:  Without a handler installed, an invalid parameter terminates the process. Each call runs in a child.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static int child_main(const char *c)
{
    FILE *volatile nf = NULL; char buf[4]; fpos_t pos; volatile int r = 0; volatile long l = 0;
    if (!strcmp(c, "fread")) r = (int)fread(buf, 1, 1, nf);
    else if (!strcmp(c, "ftell")) l = ftell(nf);
    else if (!strcmp(c, "rewind")) rewind(nf);
    else if (!strcmp(c, "fgetpos")) r = fgetpos(nf, &pos);
    else if (!strcmp(c, "feof")) r = feof(nf);
    else if (!strcmp(c, "ferror")) r = ferror(nf);
    else if (!strcmp(c, "clearerr")) clearerr(nf);
    (void)r; (void)l;
    return 77;
}
static void chk(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78);
}
TC_CASE(c_fread)    { chk("fread"); }
TC_CASE(c_ftell)    { chk("ftell"); }
TC_CASE(c_rewind)   { chk("rewind"); }
TC_CASE(c_fgetpos)  { chk("fgetpos"); }
TC_CASE(c_feof)     { chk("feof"); }
TC_CASE(c_ferror)   { chk("ferror"); }
TC_CASE(c_clearerr) { chk("clearerr"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(c_fread); TC_RUN(c_ftell); TC_RUN(c_rewind); TC_RUN(c_fgetpos);
    TC_RUN(c_feof); TC_RUN(c_ferror); TC_RUN(c_clearerr);
    return tc_end();
}
