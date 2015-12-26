#! /usr/bin/env bash

set -o errexit
set -o nounset

sudo apt-get install -y libwayland-client0 libwayland-dev libwayland-egl1 \
  libwayland-egl1-mesa libegl1-mesa-dev libgles2-mesa-dev libxcursor-dev \
  libcairo2-dev libxcb-composite0-dev libgbm-dev libxkbcommon-dev \
  libjpeg62-turbo-dev libpam0g-dev weston

ln -s /vagrant /home/vagrant/projection

# start weston-launch on startup
cat /home/vagrant/projection/platform/weston/bashrc >> /home/vagrant/.bashrc
sudo mkdir /etc/systemd/system/getty@tty1.service.d
sudo cp /home/vagrant/projection/platform/weston/autologin.conf /etc/systemd/system/getty@tty1.service.d/autologin.conf

mkdir -p /home/vagrant/.config
cp /home/vagrant/projection/platform/weston/weston.ini /home/vagrant/.config/weston.ini

sudo usermod -a -G weston-launch vagrant

sudo chown root $(which weston-launch)
sudo chmod +s $(which weston-launch)
