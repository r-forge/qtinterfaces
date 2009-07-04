
library(qtgui)
scene <- qgraphicsScene()
qsceneRect(scene)

view <- qgraphicsView(scene)
view

qsetContextMenuPolicy(view, "actions")
qsetAntialias(view, TRUE)


qscene.points(scene,
              rnorm(100), rnorm(100),
              radius = 10)

qscene.rect(scene, -5, -5, w = 10, h = 10)

qfitScene(view)

ellipse <- qgraphicsEllipseItem(9, 9, 3, 2, col = "red", fill = "yellow")
qaddItem(scene, ellipse)


## ellipse2 <- qgraphicsEllipseItem(9, 20, 3, 2, col = "red", fill = "yellow")
## qsetParentItem(ellipse, ellipse2)





lab <- qlabel("Hello world!")
labitem <- qgraphicsProxyWidget(lab)

qaddItem(scene, labitem)



library(qtdevice)
foo <- qgraphicsView(rscene <- qsceneDevice())
plot(1:5, pch = 16, cex = 3, col = "red")
itlist <- qitems(rscene, rscene$sceneRect)

qaddItem(scene, itlist[[1]])


zoominAct <- qaction(desc = "Zoom In",
                     shortcut = "Ctrl++",
                     parent = view)
zoomoutAct <- qaction(desc = "Zoom Out",
                      shortcut = "Ctrl+-",
                      parent = view)

qaddAction(view, zoominAct)
qaddAction(view, zoomoutAct)

qconnect(zoominAct,
         signal = "triggered",
         handler = function(x, ...) {
             qsetTransform(x, scale = 1.2)
         },
         user.data = view)

qconnect(zoomoutAct,
         signal = "triggered",
         handler = function(x) {
             qsetTransform(x, scale = 1/1.2)
         },
         user.data = view)

qsetDragMode(view, mode = "scroll")

