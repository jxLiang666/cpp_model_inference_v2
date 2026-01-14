prj_root=$(pwd)
build_path="$prj_root/build"
if [ -d $build_path ]; then
    rm -r build
fi
mkdir build
cd build

cmake ..
make -j4
