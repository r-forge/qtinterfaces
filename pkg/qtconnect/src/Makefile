
all:
	. createProject.sh
	make -f Makefile.qtconnect
	if test -e libqtconnect.so; then cp libqtconnect.so qtconnect.so; fi
	## for macx:
	if test -e libqtconnect.dylib; then cp libqtconnect.dylib qtconnect.so; fi


clean:
	rm -f *.so *.o *.a libqt* qtconnect.pro Makefile.qtconnect moc*.cpp


