CFLAGS := -std=gnu99

build/CMS : src/author.c src/def_struct.h src/main.c src/oc.c src/pc.c src/reviewer.c
	@if [ ! -d build ]; then mkdir build; echo "Created build directory"; fi;
	@echo "Removing old files..."
	@-rm build/*
	@echo "Building solution..."
	@gcc $^ ${CFLAGS} -o build/CMS
	@echo "Build complete. Run using ./build/CMS"
