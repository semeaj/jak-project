

#include "common/log/log.h"
#include "common/util/FileUtil.h"
#include "common/util/os.h"
#include "common/util/unicode_util.h"

#include "gtest/gtest.h"

#include "third-party/CLI11.hpp"

// Running subsets of tests, see:
// -
// https://github.com/google/googletest/blob/620659ed92829a88ee34134c782bf5b5aa5a0a0c/googletest/docs/advanced.md#running-a-subset-of-the-tests
// This can set via:
// - --gtest_filter="" CLI arg
// - 'GTEST_FILTER' environment variable,
// - or below in code by adding `::testing::GTEST_FLAG(filter) = "Test_Cases1*";` below
//
// I've set things up so VS has a run configuration that runs all tests with "Draft" in the name
// to make it easier to test a subset of tests

int main(int argc, char** argv) {
  ArgumentGuard u8_guard(argc, argv);

  // hopefully get a debug print on github actions
  setup_cpu_info();

  std::string project_path;
  CLI::App app{"OpenGOAL - GOAL Compiler Tests"};
  // gtest consumes unknown flags awkwardly, so leave anything that is not ours for gtest
  app.allow_extras();
  app.add_option("--proj-path", project_path,
                 "Specify the location of the project root. Without this the root is derived "
                 "from the executable path, which resolves to the main checkout when running "
                 "from a git worktree");
  CLI11_PARSE(app, argc, argv);

  if (!project_path.empty() && !fs::exists(project_path)) {
    lg::error("Project path override '{}' does not exist", project_path);
    return 1;
  }

  std::optional<fs::path> pp;
  if (!project_path.empty()) {
    pp = project_path;
  }
  if (!file_util::setup_project_path(pp)) {
    lg::error("Couldn't setup project path, tool is supposed to be ran in the jak-project repo!");
    return 1;
  }
  lg::initialize();

  ::testing::InitGoogleTest(&argc, argv);

  // Re-init failed folder
  std::string failedFolder = file_util::get_file_path({"test/goalc/source_generated/failed/"});
  if (fs::exists(failedFolder)) {
    fs::remove_all(failedFolder);
  }
  fs::create_directory(failedFolder);

  return RUN_ALL_TESTS();
}
