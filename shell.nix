{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = [ pkgs.cmake pkgs.gcc pkgs.clang pkgs.ninja pkgs.gcovr pkgs.cppcheck pkgs.pkg-config ];
}
