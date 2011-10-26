files="kern/boot/boot"
fd = open(files,'r')
str = fd.read()
strlen = len(str);
strlen = 510 - strlen
fd.close()
fd = open(files,'a');
fd.write('\x00'*strlen)
fd.write('\x55')
fd.write('\xAA')
fd.close()
