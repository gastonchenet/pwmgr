#!/bin/bash

APPNAME="pwmgr"

# Compile the program
gcc -o main main.c

# Make the program directory
mkdir -p $HOME/.${APPNAME}

# Move the program to the /home directory
mv main $HOME/.${APPNAME}/pwmgr

# Add the program to the PATH
refresh_command=""

case $SHELL in
  */zsh)
    echo -e "\n# PWMGR\nexport PATH=\$PATH:$HOME/.${APPNAME}" >> $HOME/.zshrc
    refresh_command="exec $SHELL -l"
    ;;
  */bash)
    echo -e "\n# PWMGR\nexport PATH=\$PATH:$HOME/.${APPNAME}" >> $HOME/.bashrc
    refresh_command="source $HOME/.bashrc"
    ;;
  */fish)
    echo -e "\n# PWMGR\nset -x PATH \$PATH $HOME/.${APPNAME}" >> $HOME/.config/fish/config.fish
    refresh_command="source $HOME/.config/fish/config.fish"
    ;;
  *)
    echo "Unsupported shell"
    exit 1
    ;;
esac

# Refresh the shell
eval $refresh_command

echo "PWMGR has been installed successfully"
