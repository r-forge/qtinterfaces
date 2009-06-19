
all:
	sh createProject.sh
	make -f Makefile.qtdevice
	if test -e libqtdevice.so; then cp libqtdevice.so qtdevice.so; fi
	## for macx:
	if test -e libqtdevice.dylib; then cp libqtdevice.dylib qtdevice.so; fi


clean:
	rm -f *.so *.o *.a libqt* qtdevice.pro Makefile.qtdevice moc*.cpp


