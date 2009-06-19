

library(qtconnect)
foo <- qpushButton()
qsetDeleteOnClose(foo, TRUE)
foo

bar <-
    qconnect(foo, user.data = foo,
             handler = function(x) {
                 if (qtext(x) == "Yeah!")
                     qsetText(x, "Done.")
                 else
                     qsetText(x, "Yeah!")
             }, which = "clicked_bool")

baz <-
    qconnect(foo, user.data = foo,
             button.click = function(x) {
                 print(class(x))
                 print(qtext(x))
             })


zab <-
    qconnect(foo, NULL,
             destroyed = function(x) {
                 print("destroyed")
             })



qdisconnect(foo, bar) # disconnect bar only

qdisconnect(foo)

qconnect(foo, user.data = foo,
         handler = function(x) {
             print(class(x))
             print(qtext(x))
         }, which = "clicked_bool")

qdisconnect(foo)

## connect to cell double-click in tables

ipkgs <- installed.packages()

library(qtmisc)
vpkgs <- qdataview(ipkgs)
qsetSortingEnabled(vpkgs, FALSE)
qresizeColumnsToContents(vpkgs)
vpkgs

qconnect(vpkgs, user.data = list(ipkgs = ipkgs, vpkgs = vpkgs),
         handler = function(x) {
             i <- qcurrentRow(x$vpkgs)
             j <- qcurrentColumn(x$vpkgs)
             print(x$ipkgs[i, j])
             old.pager <- getOption("pager")
             on.exit(options(pager = old.pager))
             options(pager = qpager)
             print(help(package = x$ipkgs[i, "Package"]))
         }, which = "cellDoubleClicked_int_int")

## qdisconnect(vpkgs)



