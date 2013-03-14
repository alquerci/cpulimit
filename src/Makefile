EXEC = $(SD_BUILD)/$(ID_EXEC)/$(EXENAME)
D_BUILD = $(SD_BUILD)/$(ID_EXEC)
SRC = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

CMAKE_DIR = $(SD_DIST)/cmake
CMAKE_CONFIG = $(CMAKE_DIR)/CMakeLists.txt

all:: build

$(D_BUILD):
	$(MKDIR) "$(D_BUILD)"

$(CMAKE_DIR):
	$(MKDIR) "$(CMAKE_DIR)"

$(EXEC): $(SRC) $(HEADERS) $(D_BUILD) $(CMAKE_DIR)
	@echo -e "project($(EXENAME))" > "$(CMAKE_CONFIG)"
	@echo -e "add_executable($(EXENAME) $(SRC))" >> "$(CMAKE_CONFIG)"
	@cd "$(CMAKE_DIR)"; $(CMAKE) "$(SD_SRC)"; "$(MAKE)";
	$(CP) "$(CMAKE_DIR)/$(EXENAME).exe" "$(EXEC)"

build: $(EXEC) $(SRC) $(HEADERS)

install: build
	#$(INSTALL) -m 755 "$(EXEC)" "$(ID_ROOT)/$(ID_EXEC)/$(EXENAME)"

uninstall:
	$(RM) "$(ID_ROOT)/$(ID_EXEC)/$(EXENAME)"

clean: FORCE
	$(RM) $(EXEC)
	$(RM) -r $(CMAKE_DIR)

.PHONY: FORCE
.EXPORT_ALL_VARIABLES: