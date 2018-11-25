{ pkgs ? import <nixpkgs> {} }:
with pkgs;

let
  inherit stdenv;
  version = "1.0.0";
in
{
    katarenga = stdenv.mkDerivation rec {
        name = "katarenga-${version}";
        nativeBuildInputs = [ cmake ];
        buildInputs = [ cppzmq zeromq docopt_cpp ];
        src = ./.;
    };
}