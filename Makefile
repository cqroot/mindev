BUILD_DIR := $(CURDIR)/build

.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@echo "Building project in $(BUILD_DIR)..."
	cmake -S '$(CURDIR)' -B '$(BUILD_DIR)' -G Ninja
	cmake --build '$(BUILD_DIR)' --parallel
	@echo

.PHONY: run
run: build
	@echo "Running project..."
	'$(BUILD_DIR)/mindev'
	@echo

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: format
format:
	@echo "Running clang-format..."
	@find src -name "*.cpp" -o -name "*.h" | xargs clang-format --verbose -i -style=file
	@echo
