cmake_minimum_required(VERSION 3.13)

project(myimagedecoder)

# Шляхи до RN
if(NOT DEFINED REACT_NATIVE_DIR)
  # Знайти node_modules вгору по дереву (працює при збірці в застосунку)
  get_filename_component(PROJ_DIR "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
  set(NODE_MODULES_DIR "${PROJ_DIR}/../../node_modules")
  set(REACT_NATIVE_DIR "${NODE_MODULES_DIR}/react-native")
endif()

set(REACT_ANDROID_DIR "${REACT_NATIVE_DIR}/ReactAndroid")
set(JSI_DIR "${REACT_NATIVE_DIR}/ReactCommon/jsi")

add_library(myimagedecoder SHARED
  src/main/cpp/myimagedecoder.cpp
)

# Заголовки
target_include_directories(myimagedecoder PRIVATE
  "${JSI_DIR}"
)

# Імпортуємо prebuilt .so з RN
add_library(jsi SHARED IMPORTED)
set_target_properties(jsi PROPERTIES IMPORTED_LOCATION
  "${REACT_ANDROID_DIR}/src/main/jni/${ANDROID_ABI}/libjsi.so"
)

add_library(reactnativejni SHARED IMPORTED)
set_target_properties(reactnativejni PROPERTIES IMPORTED_LOCATION
  "${REACT_ANDROID_DIR}/src/main/jni/${ANDROID_ABI}/libreactnativejni.so"
)

find_library(log-lib log)

target_link_libraries(myimagedecoder
  jsi
  reactnativejni
  ${log-lib}
)
