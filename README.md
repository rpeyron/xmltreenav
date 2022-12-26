[![MSBuild](https://github.com/rpeyron/xmltreenav/actions/workflows/msbuild.yml/badge.svg)](https://github.com/rpeyron/xmltreenav/actions/workflows/msbuild.yml)

# xmltreenav

xmlTreeNav is a GUI for libxmldiff, and easy tree navigation :
  * fast tree navigation
  * diff XML files
  * customization of the display with XSLT files (you can customize what appears on a tree line : add a label, convert ids...)
  * XSLT HTML visualization (you can automatically apply a XSLT and view the result in xmlTreeNav ; if you copy/paste XML, it will be transformed)
  * support for large XML files
  * run both on Windows and Linux

Homepage : http://www.lprp.fr/soft/xml/xmltreenav/xmltreenav_en.php3

# Install

See prebuilt binary packages in release tab for Windows and Debian.

PPA repository for Ubuntu : [ppa:rpeyron/ppa](https://launchpad.net/~rpeyron/+archive/ubuntu/ppa)
```
sudo apt-add-repository ppa:rpeyron/ppa
```

# Build instructions

xmlTreeNav is now built by GitHub Actions continuuous integration. So you may also compile it on your own by forking this repository.

## Dependancies

xmlTreeNav depends on :
- wxWidgets  (https://www.wxwidgets.org/)
- libxml2 (http://xmlsoft.org/)
- libxmldiff (https://github.com/rpeyron/libxmldiff/)

## Linux build

xmlTreeNav is a standard autotools project :

```
./configure
make
```

If you get some problems, a `bootstrap` file is provided to rebuild all autotools.

You may install dependancies on debian systems by :
```
# Build dependancies
sudo apt-get install pkg-config libxml2-dev libxslt-dev libxmldiff libwxgtk3.0-dev
# Run dependancies
sudo apt-get install libc6 libxml2,libxslt1.1 libxmldiff libwxgtk3.0-0v5
```


## Windows build

### With Visual Studio

Windows Build is done with Visual Studio Community Edition. 
Please use the latest version in build/ (older vc version are not maintained)

vcpkg is now used for automatic dependencies installation through manifest. 

At the time I am writing this, I have not been able to use it with vcpkg installed with scoop, so you should stick to the recommanded way to install vcpkg for msbuild (ie with git clone outside project folder)


### With vscode and msys2

To build with msys2/mingw64:

You need to compile and install libxmldiff first

```
pacman -S mingw-w64-x86_64-wxwidgets3.2-msw mingw-w64-x86_64-wxwidgets3.2-common
pacman -S zip
./configure
make
```

Note: the build with msys2 won't be statically linked, so if you want to use it outside the msys2 environment, you will need to collect the required DLL to identify with:
```
ldd src/xmltreenav.exe | grep mingw
```

# Contributing

Contributions are welcome. You may use GitHub issues tracker for issues, or GitHub Pull Requests.

# Screenshots 

Diffing XML files

![Screenshot](http://www.lprp.fr/soft/xml/xmltreenav/xmltreenav_scr.jpg) 

With custom HTML display

![Screenshot](http://www.lprp.fr/soft/xml/xmltreenav/xmltreenav_scr2.jpg)
