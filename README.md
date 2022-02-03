[![Publish Docker Image](https://github.com/wiiu-env/libiosuhax/actions/workflows/push_image.yml/badge.svg)](https://github.com/wiiu-env/libiosuhax/actions/workflows/push_image.yml)

# libiosuhax
A PPC library to access IOSUHAX from PPC and a devoptab for any device or path. 
It's only compatible to RPX-Files.

## Building
Make you to have [wut](https://github.com/devkitPro/wut/) installed and use the following command for build:
```
make install
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src wiiuenv/clang-format:13.0.0-2 -r ./source ./include -i`
