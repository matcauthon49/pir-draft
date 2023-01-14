#!/bin/bash

sudo apt install -y gcc-9 g++-9

sudo apt install -y build-essential cmake libgmp-dev libglib2.0-dev libssl-dev \
                    libboost-all-dev m4 zip bubblewrap \
                    graphviz tmux bc time libomp-dev
#Check openmp is installed.
echo |cpp -fopenmp -dM |grep -i open
#Install gcc 9
sudo apt install -y gcc-9 g++-9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9
sudo update-alternatives --config gcc



build_cmake () {
  echo "Building and installing cmake from source"
  wget https://github.com/Kitware/CMake/releases/download/v3.13.4/cmake-3.13.4.tar.gz
  tar -zxvf cmake-3.13.4.tar.gz
  cd cmake-3.13.4
  sudo ./bootstrap
  sudo make
  sudo make install
  cd ..
  rm -rf cmake-3.13.4 cmake-3.13.4.tar.gz
}

if which cmake >/dev/null; then
  CMAKE_VERSION=$(cmake --version | grep -oE '[0-9]+.[0-9]+(\.)*[0-9]*')
  LATEST_VERSION=$(printf "$CMAKE_VERSION\n3.13\n" | sort | tail -n1)
  if [[ "$CMAKE_VERSION" == "$LATEST_VERSION" ]]; then
    echo "CMake already installed.."
  else
    sudo apt purge cmake
    build_cmake
  fi
else
  build_cmake
fi

ROOT="$(pwd)"
cd src

#Defining number of threads to use in parallelization
export OMP_NUM_THREADS=8
cmake CMakelists.txt -B build
cd build
make
COLOR='\033[0;35m'
NC='\033[0m' 
printf "${COLOR}---${NC} Build Complete ${COLOR}---${NC}\n"
# ./p0 &
# sleep 1
# ./p1 &
# sleep 1
# ./p2 &
# sleep 1
# ./c
wait