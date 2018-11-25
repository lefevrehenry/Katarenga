This is an implementation of the [Katarenga](http://www.parlettgames.uk/katarenga/) game.

How to build using Nix:
---------------

Installing Nix is pretty straightforward:
```
curl https://nixos.org/nix/install | sh
```
Follow the instructions displayed at the end of the script. You usually need to source a file to access the Nix commands:
```
source ~/nix-profiles/etc/profile.d/nix.sh
```

Then, in your Katarenga folder you can simply execute `nix-shell` to be in a shell with everything you need to build katarenga.
You can now execute
```
mkdir build
cd build
cmake ..
make
```
and try out this game!
