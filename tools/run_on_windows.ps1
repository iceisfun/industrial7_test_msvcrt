# Run the whole corpus on a real Windows machine.
#   1. Copy ~/test_msvcrt/build (the <crt>\*.exe tree) here.
#   2. For msvcr100/110/120: put the matching msvcrNNN.dll (VC++ redistributable) next to the exes (app-local)
#      or install the redist.  msvcr80/90 are side-by-side assemblies and need the VC++ 2005/2008 redists installed
#      (the exes carry no manifest; if they fail to load, results say CRASH rc=-1073741515 and can be ignored).
#      ucrtbase.dll ships with Windows 10+; Windows 7 needs the Universal CRT update (KB2999226).
#   3. powershell -ExecutionPolicy Bypass -File run_on_windows.ps1 -Build .\build -Out .\winresults
#   4. Copy winresults back and run:  python3 tools/tc.py ingest winresults
param([string]$Build = ".\build", [string]$Out = ".\winresults", [int]$TimeoutSec = 60)
New-Item -ItemType Directory -Force $Out | Out-Null
Get-ChildItem $Build -Directory | ForEach-Object {
  $crt = $_.Name; $od = Join-Path $Out $crt; New-Item -ItemType Directory -Force $od | Out-Null
  Get-ChildItem $_.FullName -Filter *.exe | ForEach-Object {
    $id = $_.BaseName
    $p = Start-Process -FilePath $_.FullName -WorkingDirectory $_.DirectoryName -NoNewWindow -PassThru `
         -RedirectStandardOutput (Join-Path $od "$id.out") -RedirectStandardError (Join-Path $od "$id.err")
    if (-not $p.WaitForExit($TimeoutSec * 1000)) { $p.Kill(); $rc = -999 } else { $rc = $p.ExitCode }
    $txt = Get-Content (Join-Path $od "$id.out") -ErrorAction SilentlyContinue
    @($txt) + "EXITCODE=$rc" | Set-Content (Join-Path $od "$id.txt")
    Remove-Item (Join-Path $od "$id.out"), (Join-Path $od "$id.err") -ErrorAction SilentlyContinue
  }
}
Write-Host "done: $Out"
