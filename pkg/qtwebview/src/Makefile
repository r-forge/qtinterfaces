
all:
	. createProject.sh
	make -f Makefile.qtwebview
	if test -e libqtwebview.so; then cp libqtwebview.so qtwebview.so; fi
	## for macx:
	if test -e libqtwebview.dylib; then cp libqtwebview.dylib qtwebview.so; fi


clean:
	rm -f *.so *.o *.a libqt* qtwebview.pro Makefile.qtwebview moc*.cpp


