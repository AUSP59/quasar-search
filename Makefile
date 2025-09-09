# SPDX-License-Identifier: Apache-2.0
.DEFAULT_GOAL := all
BUILD?=build
all: configure build

configure:
	cmake -S . -B $(BUILD) -D CMAKE_BUILD_TYPE=Release

build:
	cmake --build $(BUILD) -j

test:
	ctest --test-dir $(BUILD) --output-on-failure

install:
	cmake --install $(BUILD)

clean:
	rm -rf $(BUILD)

lint:
	pre-commit run --all-files || true
	yamllint -c .yamllint.yml . || true
	cmakelint $(git ls-files | grep -E '(CMakeLists\.txt|\.cmake$$)') || true
