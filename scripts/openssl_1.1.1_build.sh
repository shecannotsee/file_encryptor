#!/bin/bash
project="file_encryptor"
project_length=${#project}
# Get pwd string
path=$(pwd)
# Get last project_length chars
suffix="${path: -$project_length}"
if [ "$suffix" = "$project" ]; then
    echo "ok"
else
    echo "Path error.Please ensure that the last few strings after using pwd are "$project
    exit 1  # stop
fi
# check dir third_party
echo "Check third_party exist?"
if [ -d "./third_party" ];
then
    echo "third_party."
else
    echo "Create third_party."
fi

# get source code
git clone https://github.com/openssl/openssl.git ./third_party/openssl/ || \
  { echo "Failed to clone openssl repository."; exit 1; }
cd third_party
mv openssl openssl-src
mkdir openssl
cd openssl
install_path=$(pwd)
cd ..

# build
cd openssl-src
git checkout OpenSSL_1_1_1 || \
  { echo "Failed to checkout OpenSSL_1_1_1 of openssl."; exit 1; }
./config --prefix="$install_path" --openssldir="$install_path"/ssl || \
  { echo "config failed."; exit 1; }
make -j8 || \
  { echo "Build failed."; exit 1; }
make install || \
  { echo "Install failed."; exit 1; }
