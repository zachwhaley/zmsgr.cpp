MAKE_CMD := $(MAKE) -C build -s

all: zmsgr

zmsgr: cmake
	@$(MAKE_CMD)

samples: cmake
	@$(MAKE_CMD) $@

clean: cmake
	@$(MAKE_CMD) $@

cmake:
	@mkdir -p build
	@cd build && cmake ../

.PHONY: all zmsgr samples clean cmake
