LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_CPP_FEATURES += exceptions
LOCAL_MODULE    := MenuBase
LOCAL_SRC_FILES := main.cpp json_reader.cpp json_value.cpp json_writer.cpp iniconfig/INIFile.cpp iniconfig/INISection.cpp mod/logger.cpp mod/config.cpp Log.cpp Mod.cpp menu/Draw.cpp menu/Menu.cpp menu/CleoMenu.cpp menu/Item.cpp menu/Window.cpp Input.cpp windows/WindowTest.cpp Ped.cpp Peds.cpp Vehicle.cpp Vehicles.cpp ModConfig.cpp
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++17
LOCAL_C_INCLUDES += ./include
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)