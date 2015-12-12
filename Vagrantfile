#!/ruby
# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "debian/jessie64"

  config.vm.provider "virtualbox" do |vb|
    # Display the VirtualBox GUI when booting the machine
    #vb.gui = true

    # Customize the amount of memory on the VM:
    #vb.memory = "1024"
  end

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update -q && apt-get upgrade -qy

    install() {
      sudo apt-get install -qy "$@"
    }

    # Fix guest additions
    install dkms # xorg
    # TODO: add the CDROM or download the latest guest additions and run the
    #       installer script

    # Install wayland for testing - doesn't work because VirtualBox doesn't
    # have EGL drivers. See https://www.virtualbox.org/ticket/13471
    #install weston

    # Install scratch build dependencies
    install \
      make gcc g++ pkg-config \
      libwayland-dev libwayland-egl1-mesa libgles2-mesa-dev
  SHELL
end
