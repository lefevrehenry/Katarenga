This is an implementation of the [Katarenga](http://www.parlettgames.uk/katarenga/) game.

How to build using Nix:
---------------

Installing Nix is pretty straightforward:
```
curl https://nixos.org/nix/install | sh
```
Follow the instructions displayed at the end of the script. You usually need to source a file to access the Nix commands:
```
source ~/.nix-profile/etc/profile.d/nix.sh
```

Then you can install katarenga by executing the following command:
```
nix-env -f https://github.com/Mommessc/clem-pkgs/archive/master.tar.gz -iA katarenga
```

The installation folder will be accessible via the symbolic link `result` from where you ran the command.
