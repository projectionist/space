#! /usr/bin/env bash

set -o errexit
set -o nounset

sudo apt-get install -y git pkg-config automake libwayland-client0 libwayland-dev libwayland-egl1 \
  libwayland-egl1-mesa libegl1-mesa-dev libgles2-mesa-dev libxcursor-dev \
  libcairo2-dev libxcb-composite0-dev libgbm-dev libxkbcommon-dev \
  libjpeg62-turbo-dev libpam0g-dev weston

VAGRANT_USER_HOME=/home/vagrant

#
# The VM might be provisioned repeatedly.
# Calling this script should be idempotent.
# Place operations which should not be repeated after this comment.
#

if [ -e $VAGRANT_USER_HOME/.projectionist ]; then
  exit
fi

touch $VAGRANT_USER_HOME/.projectionist

ln -s /vagrant $VAGRANT_USER_HOME/projection

# start weston-launch on startup
cat $VAGRANT_USER_HOME/projection/platform/weston/bashrc >> $VAGRANT_USER_HOME/.bashrc
sudo mkdir /etc/systemd/system/getty@tty1.service.d
sudo cp $VAGRANT_USER_HOME/projection/platform/weston/autologin.conf /etc/systemd/system/getty@tty1.service.d/autologin.conf

mkdir -p $VAGRANT_USER_HOME/.config
cp $VAGRANT_USER_HOME/projection/platform/weston/weston.ini $VAGRANT_USER_HOME/.config/weston.ini

sudo usermod -a -G weston-launch vagrant

sudo chown root $(which weston-launch)
sudo chmod +s $(which weston-launch)
