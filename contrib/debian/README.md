
Debian
====================
This directory contains files used to package medexcoind/medexcoin-qt
for Debian-based Linux systems. If you compile medexcoind/medexcoin-qt yourself, there are some useful files here.

## medexcoin: URI support ##


medexcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install medexcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your medexcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/medexcoin128.png` to `/usr/share/pixmaps`

medexcoin-qt.protocol (KDE)

