    with import <nixpkgs> {};

    stdenv.mkDerivation {
      name = "test";
      buildInputs = [
        curl
      ];
    }
