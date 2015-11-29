
# Tuum architecture

## Directory guide

- 'lib' : Compiled system libraries output

- 'hal' : The hardware abstraction layer - contains interfaces for camera, motor drivers, RF communication etc.
  'modules' : Contained directories define linking of corresponding program module
- 'user' : Application modules for different robot logic processes

  'main' : compilation execution directory

## Build system guide

  'tlm.mk' : Makefile for top level modules
  - 'top-level-module.mk' : Makefile for building modules
    - 'platform-id.mk' : Defines all available platforms
    - 'checks.mk' : Pre-build checks
    - 'module.mk' : Defines current module targets
    - 'macros.mk' : Recursive search, assertion, modular macro definitions
      - 'os.mk' : Define MAKE_OS for current operating system
      - 'verbose.mk' : "-v" parameter verbosity control
    - 'gcc-tools.mk'
      - 'common-tools.mk'
