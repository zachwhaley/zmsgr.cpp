MAKE_CMD := $(MAKE) -C build

all: zmsgr

zmsgr: cmake
	@$(MAKE_CMD)

samples: cmake
	@$(MAKE_CMD) samples

clean: cmake
	@$(MAKE_CMD) clean

cmake:
	@mkdir -p build
	@cd build && cmake ../

.PHONY: all zmsgr samples clean cmake
