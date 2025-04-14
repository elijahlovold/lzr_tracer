# Laser Tracer
Controllable laser pointer with recording and playback compatibility

## Setup
Install the following library: [no-OS-FatFS-SD-SPI-RPi-Pico](https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico.git)

Start in the project root: 
```
mkdir lib
cd lib
git clone https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico.git
```

## Build
Need cmake and the r-pi pico sdk setup with the path env vars set

Start in the project root: 
```
mkdir build
cd build 
cmake ..
make
```

## Upload
Use [picotool](https://github.com/raspberrypi/picotool.git)

Press the boot button, plug in, and type the following: 

```
picotool load lzr_cat.uf2
picotool reboot
```
