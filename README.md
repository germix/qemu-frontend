# Qemu Frontend

![logo](proj/res/MainImage.png)

## Description

A frontend for qemu.

## Dependency

- Qt 5.10.0
  - https://www.qt.io

## Binaries

- libgcc_s_dw2-1.dll
- libstdc++-6.dll
- libwinpthread-1.dll
- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll
- styles/qwindowsvistastyle.dll
- platforms/qwindows.dll

## Languages

- English
- Spanish

## Development

### Translations

- Update

```bash
lupdate.exe proj\QemuFrontend.pro
```

- Update (No obsolete)

```bash
lupdate.exe proj\QemuFrontend.pro -no-obsolete
```

- Release

```bash
lrelease.exe proj\QemuFrontend.pro
MOVE /Y proj\translate\*.qm bin\translations\
COPY /Y proj\translate\flags\*.png bin\translations\
```
