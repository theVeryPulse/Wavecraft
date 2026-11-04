.PHONY: all debug release clean

all: debug

debug:
	mkdir -p build/debug
	cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build/debug

release:
	mkdir -p build/release
	cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
	cmake --build build/release

clean:
	rm -rf build