{inputs, ...}: {
  imports = [
    inputs.flake-parts.flakeModules.easyOverlay
  ];

  perSystem = {
    config,
    pkgs,
    ...
  }: {
    packages.default = pkgs.callPackage ./crackme.nix {};

    overlayAttrs = {
      inherit (config.packages) default;
    };
  };
}
