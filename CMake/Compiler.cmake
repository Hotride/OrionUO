# Compiler Detection and Build Specifics
if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows" AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message(FATAL_ERROR "Compiler must be clang due to use of Microsoft Specific Extensions (-fms-extensions): __declspec(property(get|set))")
endif()

if(CMAKE_GENERATOR MATCHES "Ninja")
  check_and_add_flag(DIAGNOSTICS_COLOR -fdiagnostics-color)
elseif(CMAKE_GENERATOR MATCHES "Visual Studio")
  add_compile_options("/MP")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  compile_definitions(_DEBUG DEBUG_ONLY)
  #check_and_add_flag(EXCEPTIONS /EHsc)

  # Enforce C++ standard conforming conversion rules to catch possible bugs
  add_compile_options(/permissive-)
  # Remove unreferenced inline functions/data to reduce link time and catch bugs
  add_compile_options(/Zc:inline)
  # Assume `new` (w/o std::nothrow) throws to reduce binary size
  add_compile_options(/Zc:throwingNew)
  # Enforce strict volatile semantics as per ISO C++
  add_compile_options(/volatile:iso)

  string(APPEND CMAKE_EXE_LINKER_FLAGS " /NXCOMPAT")
else()
  compile_definitions(_DEBUG DEBUG_ONLY)  
  check_and_add_flag(HAVE_WALL -Wall)

  check_and_add_flag(MSEXTENSIONS -fms-extensions) # TODO: GCC does not suppport this, fix code instead
  check_and_add_flag(IGNORED_ATTRIBUTES_DISABLE -Wno-ignored-attributes) # FIXME: due the __fastcall in the properties
  check_and_add_flag(INCONSISTENT_MISSING_OVERRIDE_DISABLE -Wno-inconsistent-missing-override) # FIXME: OnCharPress, OnKeyDown, OnLeftMouse... etc.
  check_and_add_flag(OVERLOADED_VIRTUAL_DISABLE -Wno-overloaded-virtual) # FIXME: CGameItem::GetLightID, CTextContainer::Add, CJournal::Add etc.
  check_and_add_flag(REOEDER_DISABLE -Wno-reorder) # FIXME: Initialization order in class fields
  check_and_add_flag(NULL_CONVERSION_DISABLE -Wno-null-conversion) # FIXME: NULL to bool m_CanProcessAlpha
  check_and_add_flag(SHADOW_DISABLE -Wno-shadow) # FIXME: shadowing local variables
  check_and_add_flag(SIGN_COMPARE_DISABLE -Wno-sign-compare) # FIXME
  check_and_add_flag(UNUSED_VARIABLE_DISABLE -Wno-unused-variable) # FIXME

  #check_and_add_flag(EXTRA -Wextra)
  #check_and_add_flag(MISSING_FIELD_INITIALIZERS -Wmissing-field-initializers)
  #check_and_add_flag(SWITCH_DEFAULT -Wswitch-default)
  #check_and_add_flag(FLOAT_EQUAL -Wfloat-equal)
  #check_and_add_flag(CONVERSION -Wconversion)
  #check_and_add_flag(ZERO_AS_NULL_POINTER_CONSTANT -Wzero-as-null-pointer-constant)
  
  check_and_add_flag(TYPE_LIMITS -Wtype-limits)
  check_and_add_flag(SIGN_COMPARE -Wsign-compare)
  check_and_add_flag(IGNORED_QUALIFIERS -Wignored-qualifiers)
  check_and_add_flag(UNINITIALIZED -Wuninitialized)
  check_and_add_flag(LOGICAL_OP -Wlogical-op)
  check_and_add_flag(SHADOW -Wshadow)
  check_and_add_flag(INIT_SELF -Winit-self)
  check_and_add_flag(MISSING_DECLARATIONS -Wmissing-declarations)
  check_and_add_flag(MISSING_VARIABLE_DECLARATIONS -Wmissing-variable-declarations)

  # gcc uses some optimizations which might break stuff without this flag
  #check_and_add_flag(NO_STRICT_ALIASING -fno-strict-aliasing)
  #check_and_add_flag(NO_EXCEPTIONS -fno-exceptions)

  check_and_add_flag(VISIBILITY_INLINES_HIDDEN -fvisibility-inlines-hidden)
  check_and_add_flag(VISIBILITY_HIDDEN -fvisibility=hidden)

  check_and_add_flag(FOMIT_FRAME_POINTER -fomit-frame-pointer RELEASE_ONLY)

  check_and_add_flag(GGDB -ggdb DEBUG_ONLY)
endif()

if(ENABLE_LTO)
  check_and_add_flag(LTO -flto)
  if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(CMAKE_AR gcc-ar)
    set(CMAKE_RANLIB gcc-ranlib)
  endif()
endif()

add_definitions(-DORION_CMAKE)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  add_definitions(-DORION_WINDOWS)
  set(ORION_WINDOWS 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_definitions(-DORION_LINUX)
  set(ORION_LINUX 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  add_definitions(-DORION_OSX)
  set(ORION_OSX 1)
endif()
