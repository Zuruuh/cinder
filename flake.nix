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

      nativeBuildInputs = with pkgs;[ gcc14 criterion ];
    in
    {
      devShell = pkgs.mkShell {
        inherit nativeBuildInputs;
      };

      packages = {
        default = pkgs.stdenv.mkDerivation {
          pname = "cinder";
          version = "0.1.0";
          src = ./src;

          inherit nativeBuildInputs;

          buildPhase = ''
            ls $src/**/*.c
            ${pkgs.gcc14}/bin/gcc -Wall -Werror -I$src $src/*.c $src/**/*.c -o cinder
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
