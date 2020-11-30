#!/bin/bash
#git clone https://github.com/vladimirnikitin01/antipatterns.git
# проверка существования каталога
if [[ ! -d antipatterns ]];
then
	git clone --depth 1  https://github.com/vladimirnikitin01/antipatterns.git
fi
cd antipatterns
if [[ -d antipatterns ]];
then
	cd antipatterns
fi
if [[ -d build ]];
then
	cd build
else
	mkdir build
	cd build
	cmake ..
	make
fi
a="$PWD/antipatterns"
exec $a
exit 0
