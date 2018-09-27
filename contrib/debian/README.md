
Debian
====================
This directory contains files used to package whiffd/whiff-qt
for Debian-based Linux systems. If you compile whiffd/whiff-qt yourself, there are some useful files here.

## whiff: URI support ##


whiff-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install whiff-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your whiff-qt binary to `/usr/bin`
and the `../../share/pixmaps/whiff128.png` to `/usr/share/pixmaps`

whiff-qt.protocol (KDE)

