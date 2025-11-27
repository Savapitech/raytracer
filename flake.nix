{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = {
    self,
    nixpkgs,
  }: let
    inherit (nixpkgs) lib;

    forAllSystems = function:
      lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
        "aarch64-darwin"
      ] (system: function nixpkgs.legacyPackages.${system});
  in {
    devShells = forAllSystems (pkgs: {
      default = pkgs.mkShell {
        hardeningDisable = ["fortify"];

        packages = with pkgs; [
          sfml
          compiledb
          clang
        ];

        env.NIX_CFLAGS_COMPILE =
          let
            pyVersion = lib.versions.majorMinor pkgs.python3.version;
          in
          "-isystem ${pkgs.python3}/include/python${pyVersion}";
      };
    });

    formatter = forAllSystems (pkgs: pkgs.alejandra);
  };
}
