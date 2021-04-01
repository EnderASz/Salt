# Powershell compilation script for the Salt Compiler for Windows system.

param (
    [Parameter()]
    [switch]$AllWarnings = $false,

    [Parameter()]
    [switch]$SDebug = $false
)

if($AllWarnings) {
    $wall_flag = "-Wall"
} else {
    $wall_flag = ""
}
if($SDebug) {
    $debug_flag = "-D SALT_DEBUG"
} else {
    $debug_flag = ""
}

# region - Setup before compilation

    # Set all needed paths paths and directories
    $BASEDIR = $PSScriptRoot
    $MAIN_SOURCE_PATH = "$BASEDIR\saltc.cpp"
    $RESULT_FILENAME = "saltc.exe"

    $BuildDir = "$BASEDIR\build"
    $SourcesDir = "$BASEDIR\src"
    $CompiledObjectsDir = "$BuildDir\objects"
    $BuildedMainPath = "$BuildDir\$RESULT_FILENAME"

    # Create all needed directories
    New-Item -ErrorAction Ignore -Path $CompiledObjectsDir -ItemType "directory"

    # Search all sources to build object files
    $CPPSources = Get-ChildItem -Path $SourcesDir -Include *.cpp -Recurse
    $CPPSourcesNames = ($CPPSources | %{$_.Basename})
    $CPPSourcesPaths = ($CPPSources | %{$_.FullName})

    # Set paths for future object files
    $CPPObjectsPaths = ($CPPSourcesNames | %{@("$CompiledObjectsDir\$_.o")})
# endregion

# region - Compilation
    # Compile all sources to object files
    For($i = 0; $i -lt $CPPObjectsPaths.count; $i++) {
        $CPPObjectPath = $CPPObjectsPaths[$i]
        $CPPSourcePath = $CPPSourcesPaths[$i]
        g++ -std="c++1z" $debug_flag $wall_flag -c -o $CPPObjectPath $CPPSourcePath
    }

    # Compile the main file and link with object files
    g++ -std="c++1z" $debug_flag $wall_flag -o $BuildedMainPath $MAIN_SOURCE_PATH $CPPObjectsPaths
# endregion

# region - Cleaning after compilation
    # Move the compiled result to the top folder
    Move-Item -Path $BuildedMainPath -Destination $BASEDIR -Force

    # Remove compile-time directories
    Remove-Item -Path $BuildDir -Recurse -Force
# endregion

return "$BASEDIR\$RESULT_FILENAME"
