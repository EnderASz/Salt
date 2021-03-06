# Powershell compilation script for the Salt Virtual Machine for Windows system.

# region - Setup before compilation
    $MAIN = "svm.c"
    $RESULT = "svm.exe"

    # Set all needed paths paths and directories
    $BASEDIR = $PSScriptRoot
    $BuildDir = "$BASEDIR\build"
    $SourcesDir = "$BASEDIR\src"
    $CompiledObjectsDir = "$BuildDir\objects"
    $CompiledMainPath = "$BuildDir\$RESULT"

    # Create all needed directories
    New-Item -ErrorAction Ignore -Path $CompiledObjectsDir -ItemType "directory"

    # Search all sources to build object files
    #
    # Emmm, I don't know currently what should be here,
    # so I will left empty space here now.
    # Come on, nothing more is in this file, you can leave.

    # Set paths for future object files
    #
    # Yep, here is the same situation. Still, I don't know what to do.
# endregion
