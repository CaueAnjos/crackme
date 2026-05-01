{
  description = "A bundle of crackmes to crack and test";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    systems.url = "github:nix-systems/default";
  };

  outputs = inputs @ {flake-parts, ...}:
    flake-parts.lib.mkFlake {inherit inputs;} {
      imports = [
        (import ./packages {inherit inputs;})
        ./scripts
      ];
      systems = import inputs.systems;
      perSystem = {
        self',
        pkgs,
        system,
        ...
      }: {
        _module.args.pkgs = import inputs.nixpkgs {
          inherit system;
          config = {
            allowUnfree = true;
            allowUnsupportedSystem = true;
          };
        };

        devShells.default = let
          pkgsCross = pkgs.pkgsCross.mingwW64;
        in
          pkgsCross.mkShell {
            name = "dev";
            packages = with pkgs; [
              (writeShellApplication {
                name = "ghidra";
                runtimeInputs = [ghidra];
                text = ''
                  export _JAVA_AWT_WM_NONREPARENTING=1
                  ghidra
                '';
              })
              wineWow64Packages.full
            ];

            nativeBuildInputs = with pkgsCross; [
              bear
              pkg-config
              self'.packages.cross-lua
            ];

            env = {
              PKGCONFIG = "x86_64-w64-mingw32-pkg-config";
              PKG_CONFIG_PATH = "${self'.packages.cross-lua}/lib/pkgconfig";
            };
          };
      };
    };
}
