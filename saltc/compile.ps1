# Powershell script for the Salt compiler for Windows

#Requires -RunAsAdministrator

param (
    [Parameter()]
    [switch]$AddToPath = $false,

    [Parameter()]
    [switch]$Global = $false,

    [Parameter()]
    [string]$SaltHomeDir = "$Env:ProgramFiles\Salt"
)

$MAIN = "saltc.cpp"
$RESULT = "saltc.exe"
$CURRENT = (Get-Location).Path

if($Global) {
    $Access = 2 # Machine
} else {
    $Access = 1 # User
}

# Create Salt home directory
New-Item -ErrorAction Ignore -Path $SaltHomeDir -ItemType "directory"

# Create Salt bin directory
$SaltBinDir = "$SaltHomeDir\bin"
New-Item -ErrorAction Ignore -Path $SaltBinDir -ItemType "directory"

# Search all C/C++ source files
$CPP_SOURCES = Get-ChildItem -Path "src" -Include *.cpp -Recurse
$CPP_SOURCES_NAMES = ($CPP_SOURCES | %{$_.Basename})
$CPP_SOURCES_PATHS = ($CPP_SOURCES | %{$_.FullName})
$C_SOURCES = Get-ChildItem -Path "src" -Include *.c -Recurse | %{$_.Basename}
$C_SOURCES_NAMES = ($C_SOURCES | %{$_.Basename})
$C_SOURCES_PATHS = ($C_SOURCES | %{$_.FullName})

$BuildDir = "$CURRENT\build"

# Create all needed directories
$BuildedObjectsDir = "$BuildDir\objects"
New-Item -ErrorAction Ignore -Path $BuildedObjectsDir -ItemType "directory"

# Get future related paths for object files
$CPP_OBJECTS_PATHS = ($CPP_SOURCES_NAMES | %{@("$BuildedObjectsDir\$_.o")})
$C_OBJECTS_PATHS = ($C_SOURCES_NAMES | %{@("$BuildedObjectsDir\$_.o")})

# Compile all sources to object files
For($i = 0; $i -lt $CPP_SOURCES_NAMES.count; $i++) {
    g++ -c -o $CPP_OBJECTS_PATHS[$i] $CPP_SOURCES_PATHS[$i]
}
For($i = 0; $i -lt $C_SOURCES_NAMES.count; $i++) {
    gcc -c -o $C_OBJECTS_PATHS[$i] $C_SOURCES_PATHS[$i]
}

# Compile the main file and link with object files
$CompiledMainPath = "$BuildDir\$RESULT"
g++ -o $CompiledMainPath $MAIN ($OBJECTS -join ' ')

# Move the compiled file to Salt bin
Move-Item -Path $CompiledMainPath -Destination $SaltBinDir -Force

# Clean after compilation
Remove-Item -Path "build" -Recurse -Force

# Add Salt directories to enviroment variables
if($AddToPath) {
    # Add SaltHome enviroment variable
    [Environment]::SetEnvironmentVariable("SaltHome", $SaltHomeDir, $Access)

    # Add Salt bin to PATH enviroment variable
    $PATH = [Environment]::GetEnvironmentVariables($Access).Path
    if(!$PATH -contains $SaltBinDir){

    }
    $PATH += "$SaltBinDir;"
    [Environment]::SetEnvironmentVariable("Path", $PATH, $Access)
}
