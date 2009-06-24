
all:
	bash createProject.sh
	make -f Makefile.qtgui
	if test -e libqtgui.so; then cp libqtgui.so qtgui.so; fi
	## for macx:
	if test -e libqtgui.dylib; then cp libqtgui.dylib qtgui.so; fi


clean:
	rm -f *.so *.o *.a libqt* qtgui.pro Makefile.qtgui moc*.cpp


