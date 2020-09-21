# sitara-logging
Classes for logging activity from an application

## Requirements
### Cinder
This project explicitly relies on [Cinder](https://github.com/cinder/cinder) for logging.

### Other Libraries
This library uses:
* `mongo-cxx-driver`
* `libcurl`

The easiest way to get these is via [vcpkg](https://github.com/microsoft/vcpkg/):

```
git clone https://github.com/microsoft/vcpkg/
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg.exe install mongo-cxx-driver:x64-windows curl[openssl]:x64-windows
./vcpkg.exe integrate install
```
