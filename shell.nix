{
    clem-pkgs ? import ~/Documents/Git/clem-pkgs {}
}:

clem-pkgs.katarenga.overrideAttrs (oldAttrs: rec {
    name = "katarenga-dev-env";
    buildInputs = oldAttrs.buildInputs ++ [ clem-pkgs.pkgs.qtcreator ];
})
