{
  description = "A bundle of crackmes to crack and test";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/25.11";
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
        pkgs,
        lib,
        system,
        ...
      }: {
        _module.args.pkgs = import inputs.nixpkgs {
          inherit system;
          config = {
            allowUnfree = true;
          };
        };

        devShells.default = pkgs.mkShell {
          name = "dev";
          packages = with pkgs; [
            (pkgs.writeShellApplication {
              name = "ghidra";
              runtimeInputs = [ghidra];
              text = ''
                export _JAVA_AWT_WM_NONREPARENTING=1
                ghidra
              '';
            })
          ];
        };
      };
    };
}
