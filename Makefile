MAKE_CMD := $(MAKE) -C build -s

all test: cmake
	@$(MAKE_CMD) $@

cmake:
	@mkdir -p build
	@cd build && cmake ../

clean:
	$(RM) -r build

.PHONY: clean cmake
