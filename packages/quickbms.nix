{
  llvmPackages,
  fetchFromGitHub,
  openssl,
  clang,
  zlib,
  lzo,
}:
llvmPackages.stdenv.mkDerivation {
  pname = "quickbms64";
  version = "73cb5439b22c537611f788468fa3021fae524b9b";

  src = fetchFromGitHub {
    owner = "wilson0x4d";
    repo = "quickbms";
    rev = "73cb5439b22c537611f788468fa3021fae524b9b";
    hash = "sha256-JtQZA6iFqsWEBqIs+qgEzEE7tpq/aDB5EOwYBiK6kGc=";
  };

  buildInputs = [
    openssl
    clang
    zlib
    lzo
    llvmPackages.libllvm
    llvmPackages.bintools
  ];

  buildPhase = ''
    make clean && make --jobs
  '';

  installPhase = ''
    mkdir -p "$out/bin"
    cp -r "bin" "$out"
  '';
}
