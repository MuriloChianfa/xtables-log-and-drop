#!/bin/bash

apt update
apt install -y linux-headers-$(uname -r)
apt install -y libxtables-dev
apt install -y module-assistant iptables-dev pkg-config
./configure
make all install
insmod xt_LOGD.ko

