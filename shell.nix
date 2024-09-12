{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    llvm_18
    cmake
    clang
  ];
}

