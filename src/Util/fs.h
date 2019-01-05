#pragma once

#include <experimental/filesystem>

// The purpose of this header is so that if you're on a version of C++ where filesystem is still 
// experimental then you only need to modify this file

namespace fs = std::experimental::filesystem;