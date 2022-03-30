[![Publish Docker Image](https://github.com/wiiu-env/libiosuhax/actions/workflows/push_image.yml/badge.svg)](https://github.com/wiiu-env/libiosuhax/actions/workflows/push_image.yml)

# libiosuhax
A PPC library to access IOSUHAX from PPC and a devoptab for any device or path. 
It's only compatible to RPX-Files.

## Building
Make you to have [wut](https://github.com/devkitPro/wut/) installed and use the following command for build:
```
make install
```
Note:
You might have to build the latest wut from source if you use the prebuild devkitpro packages until it has a new release.
It won't work with wut versions like wut 1.0.0-beta12 and older.

## Use this lib in Dockerfiles.
A prebuilt version of this lib can found on dockerhub. To use it for your projects, add this to your Dockerfile.
```
[...]
COPY --from=wiiuenv/libiosuhax:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://hub.docker.com/r/wiiuenv/libiosuhax/tags). 
It's highly recommended to pin the version to the **latest date** instead of using `latest`.

## Format the code via docker

`docker run --rm -v ${PWD}:/src wiiuenv/clang-format:13.0.0-2 -r ./source ./include -i`
