# Powershell compilation script for the Salt Compiler for Windows system.

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
    $CSources = Get-ChildItem -Path $SourcesDir -Include *.c -Recurse
    $CSourcesNames = ($CSources | %{$_.Basename})
    $CSourcesPaths = ($CSources | %{$_.FullName})

    # Set paths for future object files
    $CPPObjectsPaths = ($CPPSourcesNames | %{@("$CompiledObjectsDir\$_.o")})
    $CObjectsPaths = ($CSources | %{@("$CompiledObjectsDir\$_.o")})
    $AllObjectsPaths = $CPPObjectsPaths + $CObjectsPaths
# endregion

# region - Compilation
    # Compile all sources to object files
    For($i = 0; $i -lt $CPPObjectsPaths.count; $i++) {
        g++ -c -o $CPPObjectsPaths[$i] $CPPSourcesPaths[$i]
    }
    For($i = 0; $i -lt $CSourcesPaths.count; $i++) {
        gcc -c -o $CObjectsPaths[$i] $CSourcesPaths[$i]
    }

    # Compile the main file and link with object files
    g++ -o $BuildedMainPath $MAIN_SOURCE_PATH $AllObjectsPaths
# endregion

# region - Cleaning after compilation
    # Move the compiled result to the top folder
    Move-Item -Path $BuildedMainPath -Destination $BASEDIR -Force

    # Remove compile-time directories
    Remove-Item -Path $BuildDir -Recurse -Force
# endregion

return "$BASEDIR\$RESULT_FILENAME"
