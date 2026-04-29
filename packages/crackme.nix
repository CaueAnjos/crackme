{
  stdenv,
  lua,
  pkg-config,
}:
stdenv.mkDerivation rec {
  pname = "crackme";
  version = "beta-0.0.0";

  src = ../src;

  nativeBuildInputs = [pkg-config];
  buildInputs = [lua];

  installPhase = ''
    mkdir -p "$out/bin"
    cp "${pname}.exe" "$out/bin"
  '';
}
