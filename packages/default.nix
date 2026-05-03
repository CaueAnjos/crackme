{inputs, ...}: {
  imports = [
    inputs.flake-parts.flakeModules.easyOverlay
  ];

  perSystem = {
    self',
    config,
    pkgs,
    ...
  }: let
    crossPkgs = pkgs.pkgsCross.mingwW64;
  in {
    packages = {
      quickbms = pkgs.callPackage ./quickbms.nix {};
      protonhax = pkgs.callPackage ./protonhax.nix {};

      cross-lua = crossPkgs.lua.overrideAttrs (old: {
        outputs = ["out"];
        makeFlags =
          old.makeFlags
          ++ [
            "MYLIBS="
          ];
        postPatch =
          old.postPatch
          + ''
            substituteInPlace src/Makefile \
              --replace 'strip --strip-unneeded' \
                '${crossPkgs.stdenv.cc.targetPrefix}strip --strip-unneeded'
          '';
        installPhase = ''
          mkdir -p $out/bin $out/include $out/lib/pkgconfig

          install -m755 src/lua.exe $out/bin/lua.exe
          install -m755 src/luac.exe $out/bin/luac.exe
          install -m644 src/lua.h src/lualib.h src/lauxlib.h src/luaconf.h $out/include
          install -m644 src/liblua.a $out/lib

          cat > $out/lib/pkgconfig/lua.pc << EOF
          prefix=$out
          exec_prefix=\''${prefix}
          libdir=\''${prefix}/lib
          includedir=\''${prefix}/include

          Name: Lua
          Description: Lua scripting language
          Version: 5.4
          Libs: -L\''${libdir} -llua
          Cflags: -I\''${includedir}
          EOF
        '';
        setupHook = null;
      });
      default =
        (crossPkgs.callPackage ./crackme.nix {
          lua = self'.packages.cross-lua;
        }).overrideAttrs {
          makeFlags = [
            "PKGCONFIG=x86_64-w64-mingw32-pkg-config"
          ];
        };
    };

    overlayAttrs = {
      inherit (config.packages) default;
    };
  };
}
