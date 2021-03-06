# Powershell compilation script for the Salt Compiler for Windows system.

# region - Setup before compilation
    $MAIN = "saltc.cpp"
    $RESULT = "saltc.exe"

    # Set all needed paths paths and directories
    $BASEDIR = $PSScriptRoot
    $BuildDir = "$BASEDIR\build"
    $SourcesDir = "$BASEDIR\src"
    $CompiledObjectsDir = "$BuildDir\objects"
    $CompiledMainPath = "$BuildDir\$RESULT"

    # Create all needed directories
    New-Item -ErrorAction Ignore -Path $CompiledObjectsDir -ItemType "directory"

    # Search all sources to build object files
    $CPPSources = Get-ChildItem -Path $SourcesDir -Include *.cpp -Recurse
    $CPPSourcesNames = ($CPP_SOURCES | %{$_.Basename})
    $CPPSourcesPaths = ($CPP_SOURCES | %{$_.FullName})
    $CSources = Get-ChildItem -Path $SourcesDir -Include *.c -Recurse
    $CSourcesNames = ($C_SOURCES | %{$_.Basename})
    $CSourcesPaths = ($C_SOURCES | %{$_.FullName})

    # Set paths for future object files
    $CPP_OBJECTS_PATHS = ($CPP_SOURCES_NAMES | %{@("$CompiledObjectsDir\$_.o")})
    $C_OBJECTS_PATHS = ($C_SOURCES_NAMES | %{@("$CompiledObjectsDir\$_.o")})
    $ALL_OBJECTS_PATHS = $CPP_OBJECTS_PATHS + $C_OBJECTS_PATHS
# endregion

# region - Compilation
    # Compile all sources to object files
    For($i = 0; $i -lt $CPP_OBJECTS_PATHS.count; $i++) {
        g++ -c -o $CPP_OBJECTS_PATHS[$i] $CPP_SOURCES_PATHS[$i]
    }
    For($i = 0; $i -lt $C_SOURCES_NAMES.count; $i++) {
        gcc -c -o $C_OBJECTS_PATHS[$i] $C_SOURCES_PATHS[$i]
    }

    # Compile the main file and link with object files
    g++ -o $CompiledMainPath $MAIN ($ALL_OBJECTS_PATHS -join ' ')
# endregion

# region - Cleaning after compilation
    # Move the compiled result to the top folder
    Move-Item -Path $CompiledMainPath -Destination $BASEDIR -Force

    # Remove compile-time directories
    Remove-Item -Path $BuildDir -Recurse -Force
# endregion

return $CompiledMainPath
