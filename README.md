# sitara-logging
Classes for logging activity from an application

## Requirements
### cinder
This project explicitly relies on [Cinder](https://github.com/cinder/cinder) for logging.

### mongo-cxx-driver
This library uses `mongo-cxx-driver` to write to mongo databases.

The easiest way to get this working is via [vcpkg](https://github.com/microsoft/vcpkg/):

```
git clone https://github.com/microsoft/vcpkg/
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg.exe install mongo-cxx-driver
./vcpkg.exe integrate install
```
