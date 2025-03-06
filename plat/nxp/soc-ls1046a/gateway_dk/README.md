### Building the images

To build BL2 for Gateway Development Kit specifically, you first need to have the Reset Configuration Word (RCW) ready, so build that first. Once done, run the following (`DEBUG=1` flag is optional):

```
$ make PLAT=gateway_dk RCW=../mono-rcw/gateway_dk/NN_FFSSPSNP_1133_5A06/rcw_1600_qspiboot.bin DEBUG=1 pbl
```

Built files can be found in `build/gateway_dk/release|debug`, the one we use to flash our board is `bl2_qspi.pbl` - this file needs to be flashed at offset `0x0` on the NOR flash.

To build BL3 (u-boot), run:

```
$ make PLAT=gateway_dk BOOT_MODE=qspi BL33=../u-boot/u-boot.bin DEBUG=1 fip
```