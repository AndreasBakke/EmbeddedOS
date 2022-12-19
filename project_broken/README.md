# Micrium meets Landtiger

[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Build Status](http://img.shields.io/travis/badges/badgerbadgerbadger.svg?style=flat-square)](https://travis-ci.org/badges/badgerbadgerbadger) 

The RTOS Micrium-C OS III ported to Landtiger board.

![image](landtiger_board.jpg)

Prof. Paolo Bernardi  [paolo.bernardi@polito.it](mailto:paolo.bernardi@polito.it), Computer Architectures, Politecnico di Torino, A.Y. 2019/2020.

Student:  Francesco Angione  [s262620@studenti.polito.it](mailto:s262620@studenti.polito.it)

## Table of Contents 


- [Setup](#setup)
- [OS Architecture](#architecture)
- [Documentation](#documentation)
- [Contribution](#contribution)
- [License](#license)


### Clone
 Clone this repo to your local machine using and feel free to contribute!
  ```shell
  $ git clone http://cas.polito.it/gitlab/nxp-landtiger/modulated-led-control/micrium-os-landtiger.git
  ```
  

### Setup
You need to have installed Keil uVision. A license is needed for compiling the OS.
  
### Architecture 
In the image an architecture of Micrium-C OS III is presented.

![image](uC_arch.PNG)

- 1 The port developer is responsible for providing uC/OS-III CPU specific portion. A uC/OS-III prt consist of writing or changing the contents of four kernel specific files: os_cpu.h, os_cpu_a.asm, os_cpu_a.inc and os_cpu_c.c
- 2 A port also involves writing or changing the contents of two CPU specific files: cpu.h, cpu_a.asm. Cpu_core.c is generally generic and should not require any modifications. 
- 3 A Board Support Package (BSP) is generally necessary to interface uC/OS-III to a timer (which is used for the clock tick and an interrupt controller).
- 4 Some of the semiconductor manufacturers provide source and header files to acess on-chip peripherals. These are contained in CPU/MCU specific files. You generally don't need to modify any of these and thus, you can use them as-is.

The application code is the code developed for a specific application of the OS and the ported board.

  
## Documentation
For a more detailed information see about <a href="https://os.mbed.com/media/uploads/wim/landtiger_v2.0_-_manual__v1.1.pdf" target="_blank">`Landtiger Board`</a>.

For a more detailed information see about <a href="https://www.nxp.com/docs/en/user-guide/UM10360.pdf" target="_blank">`LPC1768 core`</a>.

For a more detailed information see about  <a href="https://doc.micrium.com/display/ucos/" target="_blank">`Micrium C-OS`</a>.

## Contribution
Special thanks to:
- Cinelli Ermete  [s235002@studenti.polito.it](mailto:s235002@studenti.polito.it)
- Franco Alessandro  [s245574@studenti.polito.it](mailto:s245574@studenti.polito.it)

For fixing my bugs.

## Useful Link 

Reach out the source code of Micrium-C OS III 

-  <a href="https://github.com/Micrium/uC-OS3">`Micrium C-OS III kernel`</a>
-  <a href="https://github.com/Micrium/uC-CPU">`Micrium C-OS CPU module `</a>
-  <a href="https://github.com/Micrium/uC-Shell">`Micrium C-OS shell module `</a>
-  <a href="https://github.com/Micrium/uC-Clk">`Micrium C-OS clock module`</a>
-  <a href="https://github.com/Micrium/uC-Common">`Micrium C-OS common module`</a>
-  <a href="https://github.com/Micrium/uC-LIB">`Micrium C-OS lib module`</a>

## License
GNU General Public License v3.0



