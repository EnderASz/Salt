# This is the Powershell installation script for 
# the Salt Compiler and the Salt Virtual Machine.
# When running this you should know that it will
# compile everything from the start.
#
# After compilation, this moves the compiled files to created
# a "Salt Home" directory [default it's 'ProgramFiles\Salt' directory].
#
# If used '-SetSystemVariables' and/or '-SetUserVariables' flag,
# after compilation, this sets a couple of system variables.

#Requires -RunAsAdministrator
param (
    [Parameter()]
    [switch]$SetSystemEnvVars = $false,

    [Parameter()]
    [switch]$SetUserEnvVars = $false,

    [Parameter()]
    [string]$SaltHomeDir = "$Env:ProgramFiles\Salt"
)

# region - Setup installer
    $BASEDIR = $PSScriptRoot
    $SaltcCompilerScriptPath = "$BASEDIR\saltc\compile.ps1"
    $SVMCompilerScriptPath = "$BASEDIR\svm\compile.ps1" #TODO
# endregion

# region - Compile Salt Compiler
    echo "Compiling Salt Compiler"
    $SaltCompilerBinPath = (& $SaltcCompilerScriptPath)
# endregion

# region - Compile Salt Virtual Machine - #TODO
    # echo "Compiling Salt Virtual Machine"
    # $SVMBinPath = (& $SVMCompilerScriptPath)
# endregion

# region - Building 'SaltHome' directory
    # Create Salt home directory
    New-Item -ErrorAction Ignore -Path $SaltHomeDir -ItemType "directory"

    # region - Build Salt bin directory
        # Create Salt bin directory
        $SaltBinDir = "$SaltHomeDir\bin"
        New-Item -ErrorAction Ignore -Path $SaltBinDir -ItemType "directory"

        # region - Move saltc and svm binaries to Salt bin
        Move-Item -Path $SaltCompilerBinPath -Destination $SaltBinDir -Force
        # Move-Item -Path $SVMBinPath -Destination $SaltBinDir -Force #TODO
    # endregion
# endregion

# region - Set couple enviroment variables
    # Add Salt directories to user enviroment variables
    if($SetUserEnvVars) {
        # Add 'SaltHome' user enviroment variables
        [Environment]::SetEnvironmentVariable("SaltHome", $SaltHomeDir, 1)

        # Add Salt bin to 'PATH' user enviroment variable
        $PATH = [Environment]::GetEnvironmentVariables(1).Path
        if(!$PATH -contains $SaltBinDir){
            $PATH += "$SaltBinDir;"
            [Environment]::SetEnvironmentVariable("Path", $PATH, 1)
        }
        
    }

    # Add Salt directories to system enviroment variables
    if($SetSystemEnvVars) {
        # Add SaltHome to system enviroment variables
        [Environment]::SetEnvironmentVariable("SaltHome", $SaltHomeDir, 2)

        # Add Salt bin to PATH enviroment variable
        $PATH = [Environment]::GetEnvironmentVariables(2).Path
        if(!$PATH -contains $SaltBinDir){
            $PATH += "$SaltBinDir;"
            [Environment]::SetEnvironmentVariable("Path", $PATH, 2)
        }
    }
#endregion
