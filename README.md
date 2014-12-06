# CATernel #
## CATernel is research purpose uKernel over x86 Architecture, so far.. ##
---

![Logo](https://pbs.twimg.com/media/BDEaTlsCEAEfcI7.png:large)
![Screenshot](http://www.mediafire.com/convkey/d335/7ybghgqedh812qefg.jpg?size_id=5)
---

### How is it built ###
* Building
    - `$ git clone https://github.com/SaadTalaat/CATernel.git`
    download source code
    - `$ make image`
    outputs image into 'kern/kernel/CATernel.img`
    - `$ make clean`
    removes the kern output directory.
    - `$ make iso`
    generates iso image of the kernel

---

### Usage ###
CATernel could work either on emulators or x86 Hardware
* Bochs: 
   - `bochs`
   If you are in source directory, you only need to run bochs
   If you are on independent directory make sure you copy bochsrc or use it as configuration file
* Qemu: 
   - `qemu-system-i386 -hda kern/kernel/CATernel.img`
   Run as hard disk image
   - `qemu-system-i386 -cdrom kern/CATernel.iso`
   Run as iso
* VirtualBox: 
   CATernel can be ran over VirtualBox only using cd iso image, set it as CD Drive and start live session

---

### How to Contribute ###
* Contribution:
requires good familarity with OS concepts and implementation. Documentation available on CATernel-Doc repo, you can contribute as far as you can understand the project structure. You'll be using:-
* Git [Obviously]
* x86 emulator (bochs/qemu)
* gnu autotools, binutils

---
	 
### To be supported ###
* Refer to TODO

---

### Notes ###
* Please make your commits descriptive.
* http://github.com/SaadTalaat/CATernel-Doc [Documentation]
* http://catreloaded.net [ Organization ]
* http://twitter.com/catreloaded
* http://facebook.com/catreloaded.team
* if you are facing any problem get on IRC (Freenode) channel #CATReloaded.

---

### Authors ###
* Saad Talaat  saadtalaat[at]gmail[dot]com
* Menna Essa   http://twitter.com/MennaEssa
