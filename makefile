# Disable all implicit rules
.SUFFIXES:

# Compiler and Linker
CXX :=g++
# Compiler and linker flags
# CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Og -g -pipe -Wall -fPIC -MMD -MP
# CXXFLAGS := -std=c++17 -Wall -Wextra -O0 -g -pipe -fPIC -MMD -MP
#CXXFLAGS := -std=c++17 -Wall -Wextra -Og -g -pipe -fPIC -MMD -MP -fuse-ld=gold
#CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Og -g -pipe -fPIC -MMD -MP
# for dr meme check build
#CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O0 -g -pipe -fPIC -MMD -MP -static-libgcc -static-libstdc++ -ggdb -fno-omit-frame-pointer -gdwarf-2

# ok this will enable safe functions even not in debug mode, sick ! 
#CXXFLAGS := -std=c++17 -DSOL_SAFE_FUNCTION_CALLS -Wall -Wextra -Werror -Og -g -pipe -fPIC -MMD -MP
REL ?= rel
ifeq ($(REL),rel)
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -Og -g -pipe -fPIC -MMD -MP
else ifeq ($(REL),gdb)
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O0 -g -pipe -fPIC -MMD -MP 
else ifeq ($(REL),drmem)
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O0 -ggdb \
            -fno-omit-frame-pointer -gdwarf-4 -pipe -fPIC \
            -MMD -MP -static-libgcc -static-libstdc++
endif

CXXLDFLAGS := #

# artifacts structure and sources
PROJECT_DIR :=$(CURDIR)
MAIN_DIR    :=$(PROJECT_DIR)/src
OBJECTS_DIR :=$(PROJECT_DIR)/objects
BIN_DIR     :=$(PROJECT_DIR)/bin

# targets
MAIN_TARGET_BASENAME :=main
MAIN_TARGET :=$(BIN_DIR)/$(MAIN_TARGET_BASENAME).exe


# make folder if not created
define WIN_MKDIR
$(shell powershell -Command "if (-not (Test-Path $(1))) { New-Item $(1) -Type Directory }")
endef
_MK_DIRS := $(call WIN_MKDIR, $(OBJECTS_DIR)) \
            $(call WIN_MKDIR, $(BIN_DIR))

define FIND_DIRS
$(shell powershell -Command "(Get-ChildItem -Path $(1) -Recurse -Directory | ForEach-Object { $$_.FullName }) -join ':' -replace '\\','/'")
endef

VPATH :=$(PROJECT_DIR):\
        $(MAIN_DIR):\
        $(call FIND_DIRS, $(MAIN_DIR)):\
        $(OBJECTS_DIR)\

JSON_INC    :=D:/Cpp/json/include
JSON_FLAGS  :=-I$(JSON_INC)

BOOST_INC     :=D:/Cpp/boost_1_83_0_install/include/boost-1_83
BOOST_FLAGS   :=-I$(BOOST_INC)
BOOST_LIB     :=D:/Cpp/boost_1_83_0_install/lib
BOOST_LDFLAGS :=-Wl,-rpath,$(BOOST_LIB) -L$(BOOST_LIB)
BOOST_LIBS    :=-lboost_filesystem-mgw13-mt-x64-1_83 \
                -lboost_thread-mgw13-mt-x64-1_83 \
                -lboost_program_options-mgw13-mt-x64-1_83

RXCPP_INC     :=D:/Cpp/rxcpp_install/install/include
RXCPP_FLAGS   :=-I$(RXCPP_INC)


LUA_INC       :=C:/msys64/mingw64/include
LUA_FLAGS     :=-I$(LUA_INC)
LUA_LIB       :=C:/msys64/mingw64/lib
LUA_LDFLAGS   :=-Wl,-rpath,$(LUA_LIB) -L$(LUA_LIB)
LUA_LIBS      :=-llua

LUA_SOL_INC   :=D:/Cpp/sol2/include
LUA_SOL_FLAGS :=-I$(LUA_SOL_INC)


RAYLIB_INC      :=C:/msys64/mingw64/include
RAYLIB_FLAGS     :=-I$(RAYLIB_INC)
RAYLIB_LIB      :=C:/msys64/mingw64/lib
RAYLIB_LDFLAGS  :=-Wl,-rpath,$(RAYLIB_LIB) -L$(RAYLIB_LIB)
RAYLIB_LIBS     :=-lraylib -DGRAPHICS_API_OPENGL_33 \
                  -lbcrypt# for boost uuid

# https://github.com/btzy/nativefiledialog-extended
NFD_INC     :=D:/Cpp/libs/nfd-install/build/include
NFD_FLAGS   :=-I$(NFD_INC)
NFD_LIB     :=D:/Cpp/libs/nfd-install/build/lib
NFD_LDFLAGS :=-Wl,-rpath,$(NFD_LIB) -L$(NFD_LIB)
NFD_LIBS    :=-lnfd

# https://github.com/dacap/clip
CLIP_INC     :=D:/Cpp/libs/clip-install/clip/include
CLIP_FLAGS   :=-I$(CLIP_INC)
CLIP_LIB     :=D:/Cpp/libs/clip-install/clip/lib
CLIP_LDFLAGS :=-Wl,-rpath,$(CLIP_LIB) -L$(CLIP_LIB)
CLIP_LIBS    :=-lclip

# for windows buildc
WINDOWS_LIB :=-lole32 -luuid -lshell32#for NFD on windows
WINDOWS_LIB +=-lws2_32#for boost process on windows

# ENTT_FLAGS :=-I$(ENTT_INC)

CXXFLAGS   +=-I$(MAIN_DIR) \
              $(JSON_FLAGS) \
              $(BOOST_FLAGS) \
              $(RXCPP_FLAGS) \
              $(LUA_FLAGS) \
              $(LUA_SOL_FLAGS) \
              $(RAYLIB_FLAGS) \
              $(NFD_FLAGS) \
              $(CLIP_FLAGS)


CXXLDFLAGS +=$(BOOST_LDFLAGS) $(BOOST_LIBS) \
             $(LUA_LDFLAGS) $(LUA_LIBS) \
             $(RAYLIB_LDFLAGS) $(RAYLIB_LIBS) \
             -lm -lpthread \
             $(NFD_LDFLAGS) $(NFD_LIBS) \
             $(WINDOWS_LIB) \
             $(CLIP_LDFLAGS) $(CLIP_LIBS)

# cpp src
define RWILDCARD
$(foreach d,$(wildcard $(1:=/*)),$(call RWILDCARD,$(d),$(2))$(filter $(subst *,%,$(2)),$(d)))
endef

SOURCES_CPP      := $(call RWILDCARD,$(MAIN_DIR),*.cpp)
OBJECTS_CPP      := $(addprefix $(OBJECTS_DIR)/,$(notdir $(SOURCES_CPP:.cpp=.o)))

MAIN_CPP         := $(filter-out %table.cpp %hoot.cpp,$(SOURCES_CPP))
MAIN_CPP_OBJECTS := $(addprefix $(OBJECTS_DIR)/,$(notdir $(MAIN_CPP:.cpp=.o)))

TABLE_CPP         := $(filter-out %main.cpp %hoot.cpp,$(SOURCES_CPP))
TABLE_CPP_OBJECTS := $(addprefix $(OBJECTS_DIR)/,$(notdir $(TABLE_CPP:.cpp=.o)))

HOOT_CPP         := $(filter-out %main.cpp %table.cpp,$(SOURCES_CPP))
HOOT_CPP_OBJECTS := $(addprefix $(OBJECTS_DIR)/,$(notdir $(HOOT_CPP:.cpp=.o)))


.PHONY: default all table info clean

default: $(MAIN_TARGET)
table:  $(TABLE_TARGET)
hoot: $(HOOT_TARGET)
all: default table hoot

$(MAIN_TARGET): $(MAIN_CPP_OBJECTS)
	$(CXX) -o $@ $^ $(CXXLDFLAGS)

$(OBJECTS_DIR)/%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

info:
	@echo "[ $(VPATH) ]"
	@echo ------------
	@echo $(CXX)

define RM_RFV # yes I know this is cursed but no "$> rm -prv" on windows
    @powershell -Command "\
    $$file=\"$(1)\";\
    if (-not (Test-Path -Path $$file)) {\
        Write-Host \"does not not exits: $$file\" -ForegroundColor Yellow;\
    } else {\
        Get-ChildItem -Path $$file| ForEach-Object {\
            if (Test-Path -Path $$_.FullName) {\
                Write-Host \"Removing: $$_\" -Foreground Red;\
                Remove-Item -Path $$_.FullName -Force;\
            } else {\
                Write-Host \"does not not exits: $$_\" -Foreground Yellow;\
            }\
        }\
    }\
"
endef

clean:
	$(call RM_RFV,$(OBJECTS_DIR)/*)
	$(call RM_RFV,$(MAIN_TARGET))

DEPENDENCIES :=$(wildcard $(OBJECTS_DIR)/*.d)
-include $(DEPENDENCIES)
