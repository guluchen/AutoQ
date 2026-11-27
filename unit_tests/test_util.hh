/*****************************************************************************
 *  AUTOQ Tree Automata Library
 *
 *  Description:
 *    Test utilities: path helper.
 *
 *****************************************************************************/

#ifndef _AUTOQ_UNIT_TESTS_TEST_UTIL_HH_
#define _AUTOQ_UNIT_TESTS_TEST_UTIL_HH_

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/**
 * Returns the path of a file/directory relative to the current test file.
 * @param file  Typically __FILE__
 * @param rel_path  Relative path from the test file's directory (e.g. "testcase/GroverFor/")
 */
inline std::string test_dir_from_file(const char* file, const std::string& rel_path = "") {
    auto base = fs::path(file).parent_path();
    return rel_path.empty() ? base.string() : (base / rel_path).string();
}

#endif
