# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure(2) do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://atlas.hashicorp.com/search.
  config.vm.box = "https://dl.dropboxusercontent.com/u/1279136/projectionist/scratch.box"

  config.vm.provider "vmware_fusion" do |v|
    # Display the  GUI when booting the machine
    v.gui = true

    # Customize the amount of memory on the VM:
    v.memory = "1024"
    v.vmx["memsize"] = "1024"
    v.vmx["numvcpus"] = "2"
  end

  config.vm.provision "shell", inline: <<-SHELL
    /vagrant/platform/weston/install.sh
    sudo apt-get install -y libglm-dev
  SHELL
end
