cmake_minimum_required(VERSION 3.13)

project(myimagedecoder)

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DLOG_TAG="myimagedecoder")

# Знайти node_modules/react-native
# (працює коли пакет використовується всередині застосунку)
get_filename_component(ANDROID_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
get_filename_component(PKG_DIR "${ANDROID_DIR}/.." ABSOLUTE)                 # .../android/..
get_filename_component(NODEMODULES_FALLBACK "${PKG_DIR}/../../node_modules" ABSOLUTE)

if (NOT DEFINED RN_ROOT)
  set(RN_ROOT "${NODEMODULES_FALLBACK}/react-native")
endif()

set(REACT_ANDROID_DIR "${RN_ROOT}/ReactAndroid")
set(JSI_INCLUDE_DIR "${RN_ROOT}/ReactCommon/jsi")

# Наше джерело
add_library(myimagedecoder SHARED
  src/main/cpp/myimagedecoder.cpp
)

# Хедери JSI
target_include_directories(myimagedecoder PRIVATE
  "${JSI_INCLUDE_DIR}"
)

# Імпортуємо prebuilt .so з ReactAndroid (вони з’являються після copyReactNativeJniLibsProjectOnly)
add_library(jsi SHARED IMPORTED)
set_target_properties(jsi PROPERTIES
  IMPORTED_LOCATION "${REACT_ANDROID_DIR}/src/main/jni/${ANDROID_ABI}/libjsi.so"
)

add_library(reactnativejni SHARED IMPORTED)
set_target_properties(reactnativejni PROPERTIES
  IMPORTED_LOCATION "${REACT_ANDROID_DIR}/src/main/jni/${ANDROID_ABI}/libreactnativejni.so"
)

# Логи
find_library(log-lib log)

# Лінкуємо
target_link_libraries(myimagedecoder
  jsi
  reactnativejni
  ${log-lib}
)
