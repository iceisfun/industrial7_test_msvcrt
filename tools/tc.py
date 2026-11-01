#!/usr/bin/env python3
"""tc.py - build / lint / run the msvcrt+ucrtbase test corpus.

  tc.py lint   [FILTER]              parse headers, check schema
  tc.py build  [FILTER] [--crt X]    compile each test for each target CRT, verify imports
  tc.py run    [FILTER] [--crt X]    run built tests (TC_WINE=/path/to/wine, WINEPREFIX honoured)
  tc.py all    [FILTER] [--crt X]    lint + build + run, summary table
  tc.py ingest DIR                   summarise results gathered on real Windows (tools/run_on_windows.ps1)
Env: TC_HAZARD=1 builds in the TC_HAZARD() cases (huge allocations that can wedge a host); TC_MEMMAX=4G per-test RAM cap
(systemd scope, 0 disables); TC_JOBS, TC_TIMEOUT, TC_WINE, TC_NATIVE.
FILTER is a substring/glob matched against test id or path.  See SCHEMA.md.
"""
import sys, os, re, subprocess, fnmatch, json, concurrent.futures as cf, shutil

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TESTS, BUILD, INC = (os.path.join(ROOT, d) for d in ("tests", "build", "include"))
CC = os.environ.get("TC_CC", "x86_64-w64-mingw32-gcc")
OBJDUMP = os.environ.get("TC_OBJDUMP", "x86_64-w64-mingw32-objdump")

# name: (generation, dll, extra cflags)
TARGETS = {
    "ucrtbase": (140, "ucrtbase.dll", ["-D_UCRT", "-D__MSVCRT_VERSION__=0xE00"]),
    "msvcr120": (120, "msvcr120.dll", ["-D__MSVCRT_VERSION__=0x1200"]),
    "msvcr110": (110, "msvcr110.dll", ["-D__MSVCRT_VERSION__=0x1100"]),
    "msvcr100": (100, "msvcr100.dll", ["-D__MSVCRT_VERSION__=0x1000"]),
    "msvcr90":  (90,  "msvcr90.dll",  ["-D__MSVCRT_VERSION__=0x0900"]),
    "msvcr80":  (80,  "msvcr80.dll",  ["-D__MSVCRT_VERSION__=0x0800"]),
    "msvcrt":   (60,  "msvcrt.dll",   []),
}
ALL = list(TARGETS)
AREAS = None  # open set; any lowercase word
FIELDS = {"id", "area", "kind", "funcs", "procs", "crt", "tier", "conf", "ref", "note"}
CONFS = {"documented", "standard", "observed", "inferred", "probe"}

def expand_crt(spec):
    out = []
    for tok in spec.split():
        if tok == "all": out += ALL
        elif tok == "win10": out += ["ucrtbase"]
        elif tok == "win7": out += [c for c in ALL if c != "ucrtbase"]  # legacy redistributables + msvcrt
        elif tok.startswith(">="): out += [c for c in ALL if TARGETS[c][0] >= int(tok[2:])]
        elif tok.startswith("<"): out += [c for c in ALL if TARGETS[c][0] < int(tok[1:])]
        elif tok in TARGETS: out.append(tok)
        else: raise ValueError("bad crt token %r" % tok)
    return [c for c in ALL if c in out]

def parse(path):
    src = open(path, encoding="utf-8", errors="replace").read()
    m = re.match(r"\s*/\*\s*TC\b(.*?)\*/", src, re.S)
    if not m: raise ValueError("no leading /* TC ... */ header")
    meta, cur = {}, None
    for line in m.group(1).splitlines():
        line = re.sub(r"^\s*\*?\s?", "", line).rstrip()
        if not line: continue
        mm = re.match(r"([a-z]+):\s*(.*)$", line)
        if mm and mm.group(1) in FIELDS: cur = mm.group(1); meta[cur] = mm.group(2).strip()
        elif mm and mm.group(1) not in FIELDS: raise ValueError("unknown field %r" % mm.group(1))
        elif cur: meta[cur] += " " + line.strip()
    return meta, src

def discover(flt=None):
    tests = []
    for dp, _, fs in os.walk(TESTS):
        for f in sorted(fs):
            if f.endswith(".c"):
                p = os.path.join(dp, f)
                if flt and not (flt in p or fnmatch.fnmatch(f[:-2], flt)): continue
                tests.append(p)
    return sorted(tests)

def lint_one(path):
    errs = []
    try: meta, src = parse(path)
    except Exception as e: return None, ["%s: %s" % (path, e)]
    base = os.path.basename(path)[:-2]
    area = os.path.basename(os.path.dirname(path))
    if meta.get("id") != base: errs.append("id %r != filename %r" % (meta.get("id"), base))
    if not re.fullmatch(r"[a-z0-9_]+", base): errs.append("id must match [a-z0-9_]+")
    if meta.get("area") != area: errs.append("area %r != directory %r" % (meta.get("area"), area))
    for req in ("id", "area", "crt", "tier", "conf", "ref"):
        if not meta.get(req): errs.append("missing field %s" % req)
    if not meta.get("funcs") and not meta.get("procs"): errs.append("need funcs and/or procs")
    try: crts = expand_crt(meta.get("crt", ""))
    except ValueError as e: errs.append(str(e)); crts = []
    if not crts: errs.append("crt expands to empty set")
    kind = meta.get("kind", "positive")
    if kind not in ("positive", "negative"): errs.append("kind must be positive|negative")
    if kind == "negative" and "_neg_" not in base: errs.append("negative tests must have '_neg_' in the id")
    if kind == "positive" and "_neg_" in base: errs.append("'_neg_' in id but kind is not negative")
    if meta.get("conf") == "probe" and not meta.get("note"): errs.append("conf: probe requires a note saying what is being probed")
    if meta.get("tier") not in ("p0", "p1", "p2"): errs.append("tier must be p0|p1|p2")
    if meta.get("conf") not in CONFS: errs.append("conf must be one of %s" % sorted(CONFS))
    if '#include "tc.h"' not in src: errs.append('missing #include "tc.h"')
    if "int main" not in src: errs.append("missing main")
    if "tc_begin()" not in src or "tc_end()" not in src: errs.append("main must call tc_begin() and tc_end()")
    ncase = len(re.findall(r"^\s*(?:TC_CASE\((?!name\))|(?!TC_)[A-Z][A-Z0-9_]*CASE\()", src, re.M)); nrun = len(re.findall(r"\bTC_RUN\(", src))
    if ncase == 0: errs.append("no TC_CASE")
    if ncase != nrun: errs.append("TC_CASE count %d != TC_RUN count %d" % (ncase, nrun))
    if ncase > 8: errs.append("too many cases (%d > 8): split the test" % ncase)
    if src.count("\n") > 220: errs.append("file too long (>220 lines): split the test")
    return dict(meta=meta, path=path, id=base, crts=crts, funcs=meta.get("funcs", "").split(),
                procs=meta.get("procs", "").split()), errs

def cflags(crt, test_id):
    gen, dll, extra = TARGETS[crt]
    return ["-O1", "-fno-builtin", "-ffp-contract=off", "-fno-fast-math", "-D__USE_MINGW_ANSI_STDIO=0",
            "-D_CRT_SECURE_NO_WARNINGS", "-D_CRT_NONSTDC_NO_DEPRECATE", "-Wno-deprecated-declarations",
            "-Wall", "-Wno-unused-function", "-Wno-format", "-Wno-unused-variable", "-Wno-unused-but-set-variable",
            "-DTC_CRT_VER=%d" % gen, '-DTC_CRT_DLL="%s"' % dll, '-DTC_TEST_ID="%s"' % test_id,
            "-I" + INC] + (["-DTC_ENABLE_HAZARD"] if os.environ.get("TC_HAZARD") == "1" else []) + extra

def link_args(crt):
    return ["-nodefaultlibs", "-lmingw32", "-lgcc", "-l" + crt, "-lmingwex", "-lmoldname", "-lkernel32"]

def imports(exe):
    """{DLL(lowercase): set(function names)} from the PE import table"""
    out = subprocess.run([OBJDUMP, "-p", exe], capture_output=True, text=True).stdout
    res, cur = {}, None
    for line in out.splitlines():
        m = re.match(r"\s*DLL Name:\s*(\S+)", line)
        if m: cur = m.group(1).lower(); res.setdefault(cur, set()); continue
        m = re.match(r"\s*[0-9a-fA-F]+\s+(?:[0-9a-fA-F]+\s+)?(?:<none>\s+|\d+\s+)?(?:[0-9a-fA-F]+\s+)?([A-Za-z_?@$][\w?@$]*)\s*$", line)
        if m and cur: res[cur].add(m.group(1))
    return res

def build_one(job):
    t, crt = job
    gen, dll, _ = TARGETS[crt]
    d = os.path.join(BUILD, crt); os.makedirs(d, exist_ok=True)
    exe = os.path.join(d, t["id"] + ".exe")
    r = subprocess.run([CC] + cflags(crt, t["id"]) + ["-o", exe, t["path"]] + link_args(crt), capture_output=True, text=True)
    if r.returncode: return dict(id=t["id"], crt=crt, status="COMPILE_FAIL", detail=(r.stderr or r.stdout)[-1500:])
    imp = imports(exe)
    extra_dlls = sorted(set(imp) - {dll, "kernel32.dll"})
    have = imp.get(dll, set())
    def satisfied(f):
        if f in have: return True
        # UCRT headers implement the printf/scanf families as inline wrappers over __stdio_common_v*
        if crt == "ucrtbase" and re.search(r"(printf|scanf)", f) and not f.startswith("__"):
            return any(i.startswith("__stdio_common_v") for i in have)
        return False
    missing = [f for f in t["funcs"] if not satisfied(f)]
    if missing:
        os.remove(exe)
        return dict(id=t["id"], crt=crt, status="NOT_IMPORTED", detail="not imported from %s: %s (absent in this CRT, inline, or resolved from libmingwex)" % (dll, " ".join(missing)))
    if extra_dlls:
        os.remove(exe)
        return dict(id=t["id"], crt=crt, status="EXTRA_DLL", detail="also imports " + " ".join(extra_dlls))
    return dict(id=t["id"], crt=crt, status="BUILT", detail="")

def wine_cmd():
    w = os.environ.get("TC_WINE")
    if w: return w
    cand = os.path.expanduser("~/work/wine/proton-ge-custom/build/dst-wine-x86_64/bin/wine")
    return cand if os.path.exists(cand) else "wine"

_UNIT_SEQ = __import__('itertools').count()

def run_one(job):
    t, crt = job
    exe = os.path.join(BUILD, crt, t["id"] + ".exe")
    if not os.path.exists(exe): return dict(id=t["id"], crt=crt, status="NOT_BUILT", lines=[])
    cmd = ([] if os.environ.get("TC_NATIVE") else [wine_cmd()]) + [exe]
    # Memory guard: huge-allocation negative tests once exhausted the host. Each run gets its own cgroup scope with a
    # hard RAM cap and no swap, so a runaway test is OOM-killed (status CRASH) instead of taking the machine down.
    # TC_MEMMAX=0 disables it. MemoryMax only: MemoryHigh throttles instead of killing and makes systemd-oomd kill the
    # whole user session.
    unit = None
    mm = os.environ.get("TC_MEMMAX", "4G")
    if mm != "0":
        unit = "tc-%d-%d" % (os.getpid(), next(_UNIT_SEQ))
        cmd = ["systemd-run", "--user", "--scope", "--quiet", "--unit=" + unit, "-p", "MemoryMax=" + mm,
               "-p", "MemorySwapMax=0", "-p", "TimeoutStopSec=3"] + cmd
    timeout = int(os.environ.get("TC_TIMEOUT", "60"))
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, errors="replace", start_new_session=True)
    try:
        out, err = p.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        # kill the whole scope (wine children included), not just the launcher, or they linger and hold the pipes open
        if unit: subprocess.run(["systemctl", "--user", "kill", "--signal=KILL", unit + ".scope"], capture_output=True)
        try: os.killpg(p.pid, 9)
        except OSError: pass
        try: p.communicate(timeout=10)
        except subprocess.TimeoutExpired: pass
        return dict(id=t["id"], crt=crt, status="TIMEOUT", lines=[])
    r = subprocess.CompletedProcess(cmd, p.returncode, out, err)
    lines = [l for l in r.stdout.splitlines() if l.startswith("TC ")]
    done = any(l.startswith("TC DONE") for l in lines)
    fails = [l for l in lines if l.startswith("TC FAIL")]
    st = "PASS" if (done and r.returncode == 0 and not fails) else ("FAIL" if fails else "CRASH")
    return dict(id=t["id"], crt=crt, status=st, rc=r.returncode, lines=fails[:6] or (["rc=%d %s" % (r.returncode, r.stderr[-200:].strip())] if st == "CRASH" else []))

def ingest(args):
    """tc.py ingest DIR   - summarise results copied back from a real Windows run (see tools/run_on_windows.ps1)"""
    d = args[0]; out = {}
    for crt in sorted(os.listdir(d)):
        cd = os.path.join(d, crt)
        if not os.path.isdir(cd): continue
        for f in sorted(os.listdir(cd)):
            if not f.endswith(".txt"): continue
            tid = f[:-4]; txt = open(os.path.join(cd, f), errors="replace").read().splitlines()
            rc = next((int(l.split("=", 1)[1]) for l in txt if l.startswith("EXITCODE=")), None)
            fails = [l for l in txt if l.startswith("TC FAIL")]
            done = any(l.startswith("TC DONE") for l in txt)
            st = "PASS" if (done and rc == 0 and not fails) else ("FAIL" if fails else "CRASH")
            out.setdefault(crt, []).append(dict(id=tid, status=st, rc=rc, lines=fails[:6]))
    from collections import Counter
    for crt, rs in out.items(): print("%-9s %s" % (crt, dict(Counter(r["status"] for r in rs))))
    json.dump(out, open(os.path.join(ROOT, "results", "windows_ingest.json"), "w"), indent=1)
    for crt, rs in out.items():
        for r in rs:
            if r["status"] != "PASS": print("  %-5s %-9s %s rc=%s\n      %s" % (r["status"], crt, r["id"], r["rc"], "\n      ".join(r["lines"])))

def main():
    args = sys.argv[1:]
    if args and args[0] == "ingest": return ingest(args[1:])
    if not args or args[0] not in ("lint", "build", "run", "all"): print(__doc__); sys.exit(2)
    cmd, args = args[0], args[1:]
    crt_sel = None
    if "--crt" in args:
        i = args.index("--crt"); crt_sel = args[i + 1].split(","); del args[i:i + 2]
    flt = args[0] if args else None
    parsed, bad = [], []
    for p in discover(flt):
        t, errs = lint_one(p)
        if errs: bad += errs
        if t: parsed.append(t)
    for e in bad: print("LINT", e)
    print("lint: %d tests, %d problems" % (len(parsed), len(bad)))
    if cmd == "lint": sys.exit(1 if bad else 0)
    jobs = [(t, c) for t in parsed for c in t["crts"] if not crt_sel or c in crt_sel]
    workers = os.cpu_count() or 4
    results = {}
    if cmd in ("build", "all"):
        with cf.ThreadPoolExecutor(workers) as ex: res = list(ex.map(build_one, jobs))
        from collections import Counter
        print("build:", dict(Counter(r["status"] for r in res)))
        for r in res:
            if r["status"] != "BUILT": print("  %-13s %-9s %s %s" % (r["status"], r["crt"], r["id"], r["detail"].strip().replace("\n", "\n      ")[:700]))
        if not flt: json.dump(res, open(os.path.join(ROOT, "results", "build.json"), "w"), indent=1)
    if cmd in ("run", "all"):
        with cf.ThreadPoolExecutor(min(workers, int(os.environ.get("TC_JOBS", "8")))) as ex: res = list(ex.map(run_one, jobs))
        from collections import Counter
        per = {}
        for r in res: per.setdefault(r["crt"], Counter())[r["status"]] += 1
        print("run:")
        for c in ALL:
            if c in per: print("  %-9s %s" % (c, dict(per[c])))
        for r in res:
            if r["status"] not in ("PASS", "NOT_BUILT"): print("  %-7s %-9s %s\n      %s" % (r["status"], r["crt"], r["id"], "\n      ".join(r["lines"])))
        if not flt: json.dump(res, open(os.path.join(ROOT, "results", "run.json"), "w"), indent=1)

if __name__ == "__main__": main()
