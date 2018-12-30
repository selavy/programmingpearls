#!/bin/bash

apt install -y vim
apt install -y git
apt install -y build-essential
apt install -y python

# # Ninja BUild
# git clone git://github.com/ninja-build/ninja.git && cd ninja
# git checkout release
# cat Re
# cat README
# ./configure.py --bootstrap
# sudo cp ninja /usr/local/bin/
# sudo cp ./misc/bash-completion /etc/bash_completion.d/

# install anaconda3
# https://www.anaconda.com/download/#linux
# curl https://repo.anaconda.com/archive/Anaconda3-5.3.1-Linux-x86_64.sh -O Anaconda3-5.3.1-Linux-x86_64.sh
# sh ./Anaconda3-5.3.1-Linux-x86_64.sh

apt install -y cmake
apt install -y htop
apt install -y curl
apt install -y tmux

# # Build NeoVim
# # NOTE(peter): Either do this BEFORE install anaconda OR remove anaconda from 
# # PATH because it is using the wrong libtool.  (or just install with package
# # manager)
git clone https://github.com/neovim/neovim
apt install -y libtool
apt install -y libtool-bin
apt install -y pkg-config
cd neovim
make CMAKE_BUILD_TYPE=Release
