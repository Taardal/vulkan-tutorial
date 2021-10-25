$cmakePath = "cmake-build-debug"
$buildPath = "${cmakePath}/Debug"
$projectPath = Get-Location

Set-Location -Path $buildPath

& ./vulkandemo.exe

Set-Location -Path $projectPath