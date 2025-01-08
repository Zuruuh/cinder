{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };

      nativeBuildInputs = with pkgs;[
        clang-tools
        clang
        gcc
        stdenv.cc.cc
        stdenv.cc.cc.lib
        stdenv.cc.libc
        stdenv.cc.libc_dev
        criterion
        glib
        pkg-config
      ];

      env = {
        LDFLAGS = "-L${pkgs.gcc13.libc}/lib";
      };
    in
    {
      devShell = pkgs.mkShell {
        inherit nativeBuildInputs env;
      };

      packages = {
        default = pkgs.stdenv.mkDerivation {
          pname = "cinder";
          version = "0.1.0";
          src = ./src;

          inherit nativeBuildInputs env;

          buildPhase = ''
            ls $src/**/*.c
            ${pkgs.stdenv.cc.cc}/bin/gcc $(${pkgs.pkg-config}/bin/pkg-config --libs --cflags glib-2.0) -Wall -Werror -I$src $src/*.c $src/**/*.c -o cinder
          '';

          installPhase = ''
            mkdir -p $out/bin
            mv cinder $out/bin
          '';
        };

        test = pkgs.stdenv.mkDerivation {
          pname = "test-cinder";
          version = "0.1.0";
          src = ./src;

          inherit nativeBuildInputs;

          buildPhase = ''
            ${pkgs.gcc14}/bin/gcc $src/test.c -o test-cinder -lcriterion
          '';

          installPhase = ''
            mkdir -p $out/bin
            mv test-cinder $out/bin
          '';
        };
      };
    });
}
