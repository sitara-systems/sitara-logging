# sitara-logging

Classes for logging activity from an application

## Requirements

### Cinder

This project explicitly relies on [Cinder](https://github.com/cinder/cinder) for logging.

### Other Libraries

This library uses:

-   `mongo-cxx-driver`
-   `libcurl`

The included `vcpkg.json` should automatically download and build the dependencies for all examples in here.  This block also requires cinder to be in an adjacent folder for the cinder dependencies to be found automatically:

    parentFolder/
        sitara-logging/
        cinder/

Use the included `vcpkg.json` as a jumping off point for adding this block into other projects!
