#!/bin/sh
# Run from root directory of the project

# unzip utility doesn't have --strip-components as tar does
# $1 - name of library
# $2 - name of zip
unzip_strip()
{
    lib=$(find ./_deps_src -name $1 -type d | wc -l)
    if [ $lib = 0 ]; then
        unzip -qn _deps_src/"$2".zip -d _deps_src/"$1"_temp/
        mkdir _deps_src/$1
        mv _deps_src/"$1"_temp/*/* _deps_src/$1
        rm -rf _deps_src/"$1"_temp 
    fi
}

###############################################################
# Main Script
###############################################################

### clean procedure request
if [ "$1" = "clean" ]; then
    rm -rf _deps_src/ft2 _deps_src/glfw3 _deps_src/glm build .cache
    echo "Cleaned directories!"
    exit 0
fi
###
main_dir=$(pwd | cat)
build_path=$1 # Get path for current BUILD_TYPE from CMake
echo $main_dir
echo $build_path
[ build_path = "" ] && echo "No specific build path was given, please enter it or \
                            press Ctrl+C to stop script: " \
                    && read build_path

# check whether script is running in the project root
top_level=$(find ./ -maxdepth 1 -name CMakeLists.txt -type f | wc -l)
if [ top_level = 0 ]; then
    echo "ERROR: Use script dev_build_deps.sh only from root directory of ViNo repository\n"
    exit 1
fi
top_level=$(head CMakeLists.txt | grep ViNo | wc -l)
if [ top_level = 0 ]; then
    echo "ERROR: Wrong directory\n"
    exit 1
fi

mkdir _deps_src 
mkdir -p $build_path/_deps/ft2 $build_path/_deps/glfw3

ft2=$(find ./_deps_src -name freetype-2* -type f | wc -l)
if [ $ft2 = 0 ]; then
    wget https://deac-fra.dl.sourceforge.net/project/freetype/freetype2/2.13.2/freetype-2.13.2.tar.xz -P _deps_src
fi
ft2=$(find ./_deps_src/ -name ft2 -type d | wc -l)
if [ $ft2 = 0 ]; then
    mkdir _deps_src/ft2
    tar -xf _deps_src/freetype-2.13.2.tar.xz -C _deps_src/ft2/ --strip-components=1
fi
# cd _deps_src/ft2/
cmake -S _deps_src/ft2/ -B $build_path/_deps/ft2/ -D BUILD_SHARED_LIBS=OFF -D CMAKE_BUILD_TYPE=Release 
cd $build_path/_deps/ft2/
cmake --build . 
cd $main_dir 

glfw3=$(find ./_deps_src -name glfw-3* -type f | wc -l)
if [ $glfw3 = 0 ]; then
    wget https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip -P _deps_src
fi
unzip_strip glfw3 glfw-3.4

# cd _deps_src/glfw3/
cmake -S _deps_src/glfw3/ -B $build_path/_deps/glfw3/ -D BUILD_SHARED_LIBS=OFF -D CMAKE_BUILD_TYPE=Release \
     -D GLFW_BUILD_WIN32=ON -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_EXAMPLES=OFF \
     -D GLFW_BUILD_DOCS=OFF
cd $build_path/_deps/glfw3/
cmake --build . 
cd $main_dir

glm=$(find ./_deps_src -name glm-1.0* -type f | wc -l)
if [ $glm = 0 ]; then
    wget https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip -P _deps_src
fi
unzip_strip glm glm-1.0.1-light
# glm can be used as header-only library, so don't actually cmake'd it
cd $main_dir