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
git clone https://github.com/ArthurSonzogni/FTXUI.git ./third_party/FTXUI/ || \
  { echo "Failed to clone FTXUI repository."; exit 1; }
cd third_party
mv FTXUI FTXUI-src
mkdir FTXUI
cd FTXUI
install_path=$(pwd)
cd ..

# build
cd FTXUI-src
git checkout v5.0.0 || \
  { echo "Failed to checkout v5.0.0 of FTXUI."; exit 1; }
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX="$install_path" ..
make -j8 || \
  { echo "Build failed."; exit 1; }
make install || \
  { echo "Install failed."; exit 1; }









