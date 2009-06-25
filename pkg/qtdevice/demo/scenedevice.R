
## demo of the scene/view-based R graphics device, which gives cheap
## pan and zoom with standard R graphics.

library(qtgui)
library(qtdevice)

foo <- qgraphicsView(qsceneDevice(10, 10))
foo

## scene is stored in attr(foo, "scene")

plot(rnorm(100), rnorm(100), pch = 16, cex = 3, col = "#AA222288")
qsetDragMode(foo, "scroll")
setAntialias(foo, TRUE)

qsetTransform(foo, scale = 1.2)
qsetTransform(foo, scale = 1/1.2)

qsetTransform(foo, rotate = 20)
qsetTransform(foo, rotate = -20)

library(qtconnect)

qsetContextMenuPolicy(foo, "actions")

zoominAct <- qaction(desc = "Zoom In",
                     shortcut = "Ctrl++",
                     parent = foo)
zoomoutAct <- qaction(desc = "Zoom In",
                      shortcut = "Ctrl+-",
                      parent = foo)

qaddAction(foo, zoominAct)
qaddAction(foo, zoomoutAct)

qconnect(zoominAct, user.data = foo,
         handler = function(x) {
             qsetTransform(x, scale = 1.2)
         }, which = "triggered_bool")

qconnect(zoomoutAct, user.data = foo,
         handler = function(x) {
             qsetTransform(x, scale = 1/1.2)
         }, which = "triggered_bool")

rscene <- attr(foo, "scene")

r <- qsceneRect(rscene)
r

qscene.text(rscene, r[1] + r[3]/2, r[2] + 0.01 * r[4],
            labels = "A demo of the <a href='http://qtinterfaces.r-forge.r-project.org'>R/Qt Interface</a>",
            html = TRUE)

