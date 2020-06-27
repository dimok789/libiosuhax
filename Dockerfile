FROM wiiuenv/devkitppc:20200625

WORKDIR tmp_build
COPY . .
RUN make clean && make -j8 && mkdir -p /artifacts/wut/usr && cp -r lib /artifacts/wut/usr && cp -r include /artifacts/wut/usr
WORKDIR /artifacts

FROM scratch
COPY --from=0 /artifacts /artifacts