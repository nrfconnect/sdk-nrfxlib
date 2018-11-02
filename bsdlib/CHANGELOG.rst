Changelog
#########
All notable changes to this project are documented in this file.

bsdlib 0.2.0
************

Updated library and header files.

- Enabled Nordic Semiconductor proprietary trace log. Increased consumption of the dedicated library RAM, indicated in bsd_platform.h.
- Resolved include of stdint.h in bsd.h.

bsdlib 0.1.0
************

Initial release.

Added
=====

- Added the following BSD Socket library variants for nrf9160, for soft-float and hard-float builds
	- libbsd_nrf9160_xxaa.a
	- liboberon_2.0.5.a (Dependency of libbsd)
