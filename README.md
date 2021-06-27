# LazyEdit

## Required Dependencies
###  Submodules
- glfw
```cpp
cd lib
rm -rf glfw
git submodule add https://github.com/glfw/glfw.git glfw
```
- FFmpeg
```
sudo apt install -y libavcodec-dev libavformat-dev libavdevice-dev libavfilter-dev
```

### Tools
- pkg-config
```
sudo apt-get update -y
sudo apt-get install -y pkg-config
```

### Commands
```
# Make configuration files
./configure.sh

# Build project
./build.sh

# Run project
./run.sh
```