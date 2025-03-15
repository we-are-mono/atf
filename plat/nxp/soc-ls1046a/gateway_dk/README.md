### Building the images

To build BL2 for Gateway Development Kit specifically, you first need to have the Reset Configuration Word (RCW) ready, so build that first. Once done, run the following (`DEBUG=1` flag is optional):

```
$ make -j8 PLAT=gateway_dk RCW=../rcw/gateway_dk/NN_FFSSPSNP_1133_5A06/rcw_1600_qspiboot.bin pbl DEBUG=1
```

Built files can be found in `build/gateway_dk/release|debug`, the one we use to flash our board is `bl2_qspi.pbl` - this file needs to be flashed at offset `0x0` on the NOR flash.

To build BL3 (u-boot), run:

```
$ make -j8 PLAT=gateway_dk BOOT_MODE=qspi BL33=../u-boot/u-boot.bin fip DEBUG=1 
```

### NOR Memory map
0x0 RCW + BL2
0x100000 BL3 + U-Boot
0x300000 Environment
0x400000 FMAN
0x500000 FDT (*.dtb) 0x10000
0x600000 Initramfs 0x500000
0x1000000 Kernel 0x1766a00

### u-boot memory map
0x82000000 kernel_addr_r  # Kernel load address
0x88000000 fdt_addr_r     # Device Tree (DTB) load address
0x88080000 ramdisk_addr_r # Initramfs load address

# Boot kernel
sf probe 0:0
sf read $kernel_addr_r 0x1000000 0x1766a00
sf read $fdt_addr_r 0x500000 0x10000
sf read $ramdisk_addr_r 0x600000 0x500000
booti $kernel_addr_r - $fdt_addr_r

=> sf probe 0:0; sf read $kernel_addr_r 0x1000000 0x1766a00; sf read $ramdisk_addr_r 0x600000 0x500000; sf read $fdt_addr_r 0x500000 0x10000; booti $kernel_addr_r $ramdisk_addr_r $fdt_addr_r

# Build initramfs
cd initramfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
cd ..
mkimage -A arm64 -O linux -T ramdisk -C gzip -d initramfs.cpio.gz initramfs.cpio.gz.uboot

# Build FIT
```
$ cp ../linux/arch/arm64/boot/Image* .
$ cp ../linux/arch/arm64/boot/dts/freescale/mono-gateway-dk.dtb .
$ cp ../initramfs.cpio.gz .
$ mkimage -f gateway.its gateway.itb
```

ip link set eth1 up
ip addr add 10.0.0.90/24 dev eth1



# Steps:


# Some commands:

mdio list
mii device mdio@fd000
mii info

mii device mdio@fd000
mii read 0x00 0x04 => 01E1
mii read 0x00 0x09 => 0200





0x00 => PHY location
0x00 => control register
0x9140 => reset

mii write 0x02 0x00 0x0140  # Disable Auto-Negotiation and Force Full-Duplex 1000Mbps
mii write 0x02 0x00 0x9140  # Reset PHY

PHY 0x00: OUI = 0x19F277, Model = 0x31, Rev = 0x00,  10baseT, HDX
PHY 0x01: OUI = 0x19F277, Model = 0x31, Rev = 0x00,  10baseT, HDX
PHY 0x02: OUI = 0x19F277, Model = 0x31, Rev = 0x00,  10baseT, HDX

• FM1 mEMAC1: 1AE_0000h
• FM1 mEMAC2: 1AE_2000h
• FM1 mEMAC3: 1AE_4000h
• FM1 mEMAC4: 1AE_6000h
• FM1 mEMAC5: 1AE_8000h
• FM1 mEMAC6: 1AE_A000h
• FM1 mEMAC9: 1AF_0000h
• FM1 mEMAC10: 1AF_2000h

setenv loadaddr 0xC0000000      # Address to load the FIT image
setenv kernel_addr_r 0xC2000000 # Kernel load address
setenv fdt_addr_r 0xC4000000    # Device Tree (DTB) load address
setenv ramdisk_addr_r 0xC6000000 # Initramfs load address
setenv ethaddr "00:04:9F:08:06:4A"
setenv eth1addr "00:04:9F:08:06:4B"
setenv eth2addr "00:04:9F:08:06:4C"
setenv eth3addr "00:04:9F:08:06:4D"
setenv eth4addr "00:04:9F:08:06:4E"




# I2C stuff:

i2c dev 0
i2c mw 0x70 3 1
i2c probe
i2c mw 0x70 0x00 0x10

The EMC2302-1 SMBus address is set at 0101_110(r/w)b.
The EMC2302-2 SMBus address is set at 0101_111(r/w)b

i2c dev 0
select 4th channel of the mux

0x01 → Channel 0 selected
0x02 → Channel 1 selected
0x04 → Channel 2 selected
0x08 → Channel 3 selected
0x10 → Channel 4 selected
0xF1 → All channels selected

### Temp data:
i2c mw 0x70 0x00 0x02 # swap to SMBus channel 1
i2c md 0x4C 0x00 1 # read temp hex, convert to decimal for celsius
i2c mw 0x70 0x00 0x04  # swap to SMBus channel 2
i2c md 0x4C 0x00 1 # read temp hex, convert to decimal for celsius

### SFP+
i2c dev 1
i2c mw 0x70 0x00 0x02
i2c probe # Shows 50 and 51, which are SFP 

# PHY stuff
mdio list
mii info

mii device mdio@fd000
1. Get MDIO device 
`mdio list` => mdio@fd000
`mii device mdio@fd000`
`mii info`
`mdio write mdio@fd000 0x00 0x00 0x1200` - reset A/N (will return back to 0x1000, which is default behavior)
`mdio read mdio@fd000 0x00 0x00`

2. write miiphy_write(MDIO_BUS, PHY_ADDR, MII_BMCR, BMCR_RESET);
phy_write(phydev, MDIO_DEVAD_NONE, MIIM_88E1310_PHY_PAGE, 0x0002);
mdio write mdio@fd000 0x00 0x1F 0x0010

mdio write mdio@fd000 0x00 0x00 0x0140
mdio write mdio@fd000 0x00 0x00 0x9140

LED control
mdio write mdio@fd000 0x00 0x27 0x0000
mdio write mdio@fd000 0x00 0x27 0x001F
mdio read mdio@fd000 0x00 0x27 0xF01E

mii write 0x00 0x27 0xF01E 
mii read mdio@fd000 0x00 0x01

3. read miiphy_read(MDIO_BUS, PHY_ADDR, MII_BMSR);
mdio read mdio@fd000 0x00 0x02
mdio read mdio@fd000 0x00 0x0


phy_reg1: 26569
phy_reg2: 57104
phy_id: 67c9df10

regnum=0x0, val=0x8000 RESET PHY
regnum=0x0, ret=0x3040 10 Mbit/Half duplex

# Set gigabit capable to 1! 
mdio write mdio@fd000 0x00 0x09 0x2000

mdio write mdio@fd000 0x00 0x0 0x8000 RESET
mdio write mdio@fd000 0x00 0x00 0x1200 RESTART AUTONEG
mdio write mdio@fd000 0x00 0x00 0x0140
0xf = register 0.15, value 0x2000 means it's 1000BASE-T Full-Duplex Capable


Koraki:
1. reset PHY chipa (0x0, 0x8000) => mdio write mdio@fd000 0x00 0x0 0x8000
2. reneg (0x0, 0x1360) => mdio write mdio@fd000 0x00 0x0 0x1360


