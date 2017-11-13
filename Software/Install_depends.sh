
sudo apt-get install libncurses5-dev libreadline-dev nettle-dev libgnutls28-dev
# Install GnuTLS 3, Readline and Nettle, Ubuntu 14.10-16.04
# sudo apt-get install libncurses5-dev libreadline-dev nettle-dev libgnutls-dev
# Install optional dependencies
# sudo apt-get install librestbed-dev libjsoncpp-dev

# Install Argon2, msgpack-c (Ubuntu 17.04+)
#sudo apt install libargon2-0-dev libmsgpack-dev

# Install python binding dependencies
#sudo apt-get install cython3 python3-dev python3-setuptools

# Build and install msgpack-c (Ubuntu 14.04 - 16.10)
sudo apt-get install build-essential cmake
wget https://github.com/msgpack/msgpack-c/releases/download/cpp-2.1.1/msgpack-2.1.1.tar.gz
tar -xzf msgpack-2.1.1.tar.gz
cd msgpack-2.1.1 && mkdir build && cd build
cmake -DMSGPACK_CXX11=ON -DMSGPACK_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=/usr ..
make -j4
sudo make install




# clone the repo
git clone https://github.com/savoirfairelinux/opendht.git

# build and install
cd opendht
mkdir build
cmake -DOPENDHT_PYTHON=OFF -DCMAKE_INSTALL_PREFIX=/usr ..
make -j4
sudo make install