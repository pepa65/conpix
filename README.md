# conpix
**Display images in 256 colors xterm**
* Author: rofl0r
* License: GNU GPLv3

## Install

### Required
* ncurses
* lepton (aka liblept aka leptonica)
* tiff
* png

### Install required packages
* OSX: Get `macports` and run:
  `sudo port install leptonica tiff libpng ncurses`
* Debian/Ubuntu:
  `sudo apt install libleptonica-dev libncurses5-dev libpng-dev libtiff-dev`

### Build
```
git clone https://github.com/pepa65/conpix
cd conpix
make
export TERM=xterm-256color
```

### Install
`sudo make install`

### Uninstall
`sudo make uninstall`

## Remarks
* The flickering at the end is caused by the restoration of the
  original colors, so the xterm is still usable afterwards.
* Repo https://github.com/rofl0r/conpix was discontinued and subsumed by
  https://github.com/rofl0r/concol in order to do a better job in C.
  The c0npix utility is included there in the `example` directory.
* The original `conpix` repo was cloned by pepa65 and will be maintained as
  a quick fix for the need to view images in the terminal, without GUI.
