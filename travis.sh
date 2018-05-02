#!/bin/bash

clang-format --version
echo Validating code formatting.

clang-format --style=file -i OrionUO/*.{h,cpp} OrionUO/*/*.{h,cpp}
dirty=$(git ls-files --modified)
if [[ $dirty ]]; then
	echo Failed. The following files are incorrectly formatted:
	echo $dirty
	echo "Diff:"
	git --no-pager diff --no-color
	exit 1
else
	echo Code formatting validation passed.
fi

echo Installing SDL 2.0.8 dependencies
sudo apt-get install --reinstall dpkg
curl http://http.us.debian.org/debian/pool/main/w/wayland/libwayland-client0_1.14.0-2_amd64.deb -O
sudo dpkg -i libwayland-client0_1.14.0-2_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/w/wayland/libwayland-server0_1.14.0-2_amd64.deb -O
sudo dpkg -i libwayland-server0_1.14.0-2_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/libd/libdrm/libdrm-common_2.4.91-2_all.deb -O
sudo dpkg -i libdrm-common_2.4.91-2_all.deb || true
curl http://http.us.debian.org/debian/pool/main/libd/libdrm/libdrm2_2.4.91-2_amd64.deb -O
sudo dpkg -i libdrm2_2.4.91-2_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/m/mesa/libgbm1_17.3.9-1_amd64.deb -O
sudo dpkg -i libgbm1_17.3.9-1_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/libx/libxkbcommon/libxkbcommon0_0.8.0-1_amd64.deb -O
sudo dpkg -i libxkbcommon0_0.8.0-1_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/libx/libxkbcommon/libxkbcommon-dev_0.8.0-1_amd64.deb -O
sudo dpkg -i libxkbcommon-dev_0.8.0-1_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/m/mesa/libegl-mesa0_17.3.9-1_amd64.deb -O
sudo dpkg -i libegl-mesa0_17.3.9-1_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/s/sndio/libsndio6.1_1.1.0-3_amd64.deb -O
sudo dpkg -i libsndio6.1_1.1.0-3_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/s/sndio/libsndio-dev_1.1.0-3_amd64.deb -O
sudo dpkg -i libsndio-dev_1.1.0-3_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/libs/libsdl2/libsdl2-dev_2.0.8+dfsg1-1_amd64.deb -O
sudo dpkg -i libsdl2-dev_2.0.8+dfsg1-1_amd64.deb || true
curl http://http.us.debian.org/debian/pool/main/libs/libsdl2/libsdl2-2.0-0_2.0.8+dfsg1-1_amd64.deb -O
sudo dpkg -i libsdl2-2.0-0_2.0.8+dfsg1-1_amd64.deb || true

echo Building Release
mkdir release && cd release && cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release && time ninja OrionUO_unity -j8
file ./OrionUO/unity/OrionUO
#./OrionUO/unity/OrionUO --headless
cd ..

echo Building Debug
mkdir debug && cd debug && cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Debug && time ninja OrionUO_unity -j8
file ./OrionUO/unity/OrionUO
#./OrionUO/unity/OrionUO --headless
