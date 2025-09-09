{
  description = "Quasar Apex dev shell (C++ toolchain, docs, QA)";
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05"; };
  outputs = { self, nixpkgs }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
    in {
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = with pkgs; [
          cmake ninja gcc clang ccache gdb valgrind doxygen graphviz
          cppcheck clang-tools python3Packages.jsonschema git gitleaks
          shfmt shellcheck editorconfig-checker
        ];
        shellHook = ''
          export CCACHE_DIR=$PWD/.ccache
          export CC="clang" CXX="clang++"
          echo "Quasar Apex devshell ready (clang, ninja, doxygen, cppcheck, clang-tidy)"
        '';
      };
    };
}
