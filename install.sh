#!/bin/bash
# This is the installation script for the salt compiler 
# and the salt virtual machine. When running this you should
# know that it will compile everything from the start.
# 
# After compilation, this sets a couple of system variables 
# and moves the compiled folders to ~/.local, creating a "salt home"
# directory there.

printf "Compiling saltc & saltvm\n"

# Compile and clean both programs
cd saltc
make && make clean
cd ..
cd svm
make && mv build/svm ./svm && make clean
cd ..

printf "Creating Salt home\n"

# Generate the Salt home directory and and a bash variable
mkdir -p $HOME/.local/salt/bin

# Export variables to users' .bashrc if they are not set
if [[ "$SALT_HOME" != "$HOME/.local/salt" ]]; then
    export SALT_HOME=$HOME/.local/salt
    echo "export SALT_HOME="$HOME"/.local/salt" >> ~/.bashrc
    echo "PATH=\$PATH:"$SALT_HOME"/bin" >> ~/.bashrc
fi

# Move binaries to bin
mv saltc/saltc $SALT_HOME/bin/saltc
mv svm/svm $SALT_HOME/bin/svm

# Copy over lib, docs & ext to salt home
cp -r doc $SALT_HOME
cp -r lib $SALT_HOME
cp -r ext $SALT_HOME

# Reload bashrc
source ~/.profile

printf "Done!\n"
