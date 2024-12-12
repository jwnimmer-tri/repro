#include <filesystem>
#include <iostream>

#include "rules_cc/cc/runfiles/runfiles.h"

using rules_cc::cc::runfiles::Runfiles;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(
      Runfiles::Create(argv[0], BAZEL_CURRENT_REPOSITORY, &error));
  if (!error.empty()) {
    std::cerr << "error: " << error << "\n";
    return 1;
  }
  std::string mesh_path = runfiles->Rlocation("example/demo/mesh.obj");
  std::string mtl_path = runfiles->Rlocation("example/demo/mesh.mtl");

  std::cout << "\nThe goal is for all of these paths to be in the same "
               "directory:\n\n";
  std::cout << "mesh_path: " << mesh_path << "\n";
  std::cout << "mtl_path: " << mtl_path << "\n\n";

  for (const auto& [key, value] : runfiles->EnvVars()) {
    if (key == "RUNFILES_DIR") {
      std::cout << "RUNFILES_DIR=" << value << " and contains these files:\n";
      const fs::path runfiles_dir{value};
      for (const auto& dir_entry :
           fs::recursive_directory_iterator(runfiles_dir)) {
        const fs::path rel_path =
            dir_entry.path().lexically_relative(runfiles_dir);
        std::cout << "  " << rel_path.string() << "\n";
      }
    }
  }

  return 0;
}
