# Powershell compilation script for the Salt Compiler for Windows system.

param (
    [Parameter()]
    [switch]$AllWarnings = $false,

    [Parameter()]
    [switch]$SDebug = $false
)

$static_flags = "-std=c++1z -Wno-unknown-pragmas"

if($AllWarnings) {
    $static_flags += " -Wall"
}
if($SDebug) {
    $static_flags += " -D SALT_DEBUG"
}

# region - Setup before compilation

    # Set all needed paths paths and directories
    Write-Host "Setting up and creating needed paths and directories..."
    $BASEDIR = $PSScriptRoot
    $MAIN_SOURCE_PATH = "$BASEDIR\saltc.cpp"
    $RESULT_FILENAME = "saltc.exe"

    $BuildDir = "$BASEDIR\build"
    $SourcesDir = "$BASEDIR\src"
    $CompiledObjectsDir = "$BuildDir\objects"
    $BuildedMainPath = "$BuildDir\$RESULT_FILENAME"

    # Create all needed directories
    New-Item -ErrorAction Ignore -Path $CompiledObjectsDir -ItemType "directory" | Out-Null

    # Search all sources to build object files
    Write-Host "Searching all sources files..."
    $CPPSources = Get-ChildItem -Path $SourcesDir -Include *.cpp -Recurse
    $CPPSourcesNames = ($CPPSources | %{$_.Basename})
    $CPPSourcesPaths = ($CPPSources | %{$_.FullName})

    # Set paths for future object files
    $CPPObjectsPaths = ($CPPSourcesNames | %{@("$CompiledObjectsDir\$_.o")})
# endregion

# region - Compilation
    # Compile all sources to object files
    For($i = 0; $i -lt $CPPObjectsPaths.count; $i++) {
        $CPPObject = $CPPObjectsPaths[$i]
        $CPPSource = $CPPSourcesPaths[$i]
        Write-Host "Compiling '$CPPSource' source file..."
        $__FILENAME__ = ($CPPSource | Resolve-Path -Relative).substring(2).replace("\", "/")
        Invoke-Expression "g++ $static_flags -c -o $CPPObject $CPPSource -D __FILENAME__=`'\`"$__FILENAME__\`"`'"
    }

    # Compile the main file and link with object files
    Write-Host "Compiling main file and linking everything needed to executable file..."
    $__FILENAME__ = ($MAIN_SOURCE_PATH | Resolve-Path -Relative).substring(2).replace("\", "/")
    Invoke-Expression "g++ $static_flags -o $BuildedMainPath $MAIN_SOURCE_PATH $CPPObjectsPaths -D __FILENAME__=`'\`"$__FILENAME__\`"`'"
# endregion

# region - Cleaning after compilation
    # Move the compiled result to the top folder
    Move-Item -Path $BuildedMainPath -Destination $BASEDIR -Force

    # Remove compile-time directories
    Remove-Item -Path $BuildDir -Recurse -Force
# endregion
Write-Host "Executable saved at: " -NoNewline
return "$BASEDIR\$RESULT_FILENAME"
