/* TC
 * id:    nfx_neg_exit_in_handler_code_preserved
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler _exit wcscpy_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exit, _cexit ("_exit terminates without calling atexit/onexit handlers"), wcscpy_s
 * note:  Handler calls _exit(123) after a wcscpy_s overflow; the registered atexit handler (writes a marker file) must not run.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

#define MARK "tc_nfx_exit_in_handler_code_preserved_marker.tmp"
static void at_exit(void)
{
    HANDLE f = CreateFileA(MARK, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f != INVALID_HANDLE_VALUE) CloseHandle(f);
}
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; _exit(123); }

TC_CASE(exit123)
{
    DWORD code = 0; int ran;
    DeleteFileA(MARK);
    ran = tc_spawn_self("ovf", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 123);
    CHECK(GetFileAttributesA(MARK) == INVALID_FILE_ATTRIBUTES);
    DeleteFileA(MARK);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        wchar_t b[2]; const wchar_t *volatile s = L"abcdef";
        atexit(at_exit);
        _set_invalid_parameter_handler(h);
        wcscpy_s(b, 2, s);
        return 77;
    }
    TC_RUN(exit123);
    return tc_end();
}
