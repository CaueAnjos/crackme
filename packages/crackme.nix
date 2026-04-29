{
  stdenv,
  lua,
  pkg-config,
}:
stdenv.mkDerivation {
  pname = "crackme";
  version = "beta-0.0.0";

  src = ../src;

  nativeBuildInputs = [pkg-config];
  buildInputs = [lua];

  installPhase = ''
    mkdir -p "$out"
    cp -r "bin" "$out"
  '';
}
