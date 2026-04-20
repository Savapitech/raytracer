{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    nixgl.url = "github:nix-community/nixGL";
  };

  outputs = {
    self,
    nixpkgs,
    nixgl,
  }: let
    inherit (nixpkgs) lib;

    forAllSystems = function:
      lib.genAttrs [
        "x86_64-linux"
        "aarch64-darwin"
      ] (system: function {
        pkgs = nixpkgs.legacyPackages.${system};
        gl = nixgl.packages.${system}; 
      });
  in {
    devShells = forAllSystems ({ pkgs, gl }: {
      default = pkgs.mkShell {
        hardeningDisable = ["fortify"];

        packages = with pkgs; [
          compiledb
          clang
          libconfig
          pkg-config
          sfml
        ];

        env.NIX_CFLAGS_COMPILE =
          let
            pyVersion = lib.versions.majorMinor pkgs.python3.version;
          in
          "-isystem ${pkgs.python3}/include/python${pyVersion}";
      };
    });

    formatter = forAllSystems ({ pkgs, ... }: pkgs.alejandra);
  };
}
