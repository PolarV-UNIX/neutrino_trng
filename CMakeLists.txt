cmake_minimum_required(VERSION 3.10)
project(neutrino_trng)

set(CMAKE_CXX_STANDARD 17)

# مسیر هدر
include_directories(include)

# ساخت کتابخانه
add_library(neutrino_trng SHARED src/neutrino_trng.cpp)

# لینک به OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(neutrino_trng OpenSSL::SSL OpenSSL::Crypto)

# نصب فایل‌ها
install(TARGETS neutrino_trng DESTINATION lib)
install(DIRECTORY include/neutrino_trng DESTINATION include)

