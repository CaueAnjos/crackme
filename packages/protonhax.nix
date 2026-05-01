{
  stdenv,
  fetchFromGitHub,
}:
stdenv.mkDerivation (finalAttrs: {
  pname = "protonhax";
  version = "1.0.5";

  src = fetchFromGitHub {
    owner = "jcnils";
    repo = "protonhax";
    rev = "${finalAttrs.version}";
    hash = "sha256-5G4MCWuaF/adSc9kpW/4oDWFFRpviTKMXYAuT2sFf9w=";
  };

  dontBuild = true;
  installPhase = ''
    mkdir -p "$out/bin"
    cp "${finalAttrs.pname}" "$out/bin/${finalAttrs.pname}"
  '';
})
