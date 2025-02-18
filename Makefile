PROJECT=Renderer
.PHONY: build

all: build

build:
	@mkdir -p build
	@cd build && make

rebuild: clean
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

clean:
	@rm -rf build

run:
	@./build/${PROJECT}
	
