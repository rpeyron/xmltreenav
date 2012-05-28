# xmltreenav

xmlTreeNav is a GUI for libxmldiff, and easy tree navigation :
  * fast tree navigation
  * diff XML files
  * customization of the display with XSLT files (you can customize what appears on a tree line : add a label, convert ids...)
  * XSLT HTML visualization (you can automatically apply a XSLT and view the result in xmlTreeNav ; if you copy/paste XML, it will be transformed)
  * support for large XML files
  * run both on Windows and Linux

Homepage : http://www.lprp.fr/soft/xml/xmltreenav/xmltreenav_en.php3

# Build instructions

## Dependancies

xmlTreeNav depends on :
- wxWidgets  (https://www.wxwidgets.org/)
- libxml2 (http://xmlsoft.org/)
- libxmldiff 

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
sudo apt-get install libc6 libxml2,libxslt1.1 libxmldiff libwxgtk3.0-0
```


## Windows build

Windows Build is done with Visual Studio Community Edition. 
Please use the latest version in build/ (older vc version are not maintained)


# Contributing

Contributions are welcome. You may use GitHub issues tracker for issues, or GitHub Pull Requests.

# Screenshots 

Diffing XML files

![Screenshot](http://www.lprp.fr/soft/xml/xmltreenav/xmltreenav_scr.jpg) 

With custom HTML display

![Screenshot](http://www.lprp.fr/soft/xml/xmltreenav/xmltreenav_scr2.jpg)
