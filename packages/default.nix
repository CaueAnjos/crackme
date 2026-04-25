{inputs, ...}: {
  imports = [
    inputs.flake-parts.flakeModules.easyOverlay
  ];

  perSystem = {
    self',
    config,
    pkgs,
    ...
  }: {
    packages.default = pkgs.callPackage ./example.nix {};
    packages.debug = self'.packages.default.override {
      format = "tux";
    };

    overlayAttrs = {
      inherit (config.packages) default;
    };
  };
}
