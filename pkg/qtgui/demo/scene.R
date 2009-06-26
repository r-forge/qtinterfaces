
library(qtgui)
scene <- qgraphicsScene()
qsceneRect(scene)

view <- qgraphicsView(scene)
view

qsetContextMenuPolicy(view, "actions")
setAntialias(view, TRUE)

zoominAct <- qaction(desc = "Zoom In",
                     shortcut = "Ctrl++",
                     parent = view)
zoomoutAct <- qaction(desc = "Zoom Out",
                      shortcut = "Ctrl+-",
                      parent = view)

qaddAction(view, zoominAct)
qaddAction(view, zoomoutAct)

zoominHandler <- 
    qconnect(zoominAct,
             signal = "triggered",
             handler = function(x, ...) {
                 qsetTransform(x, scale = 1.2)
             },
             user.data = view)

zoomoutHandler <- 
    qconnect(zoomoutAct,
             signal = "triggered",
             handler = function(x) {
                 qsetTransform(x, scale = 1/1.2)
             },
             user.data = view)


qsetDragMode(view, mode = "scroll")


qscene.points(scene,
              rnorm(100), rnorm(100),
              radius = 10)

qscene.rect(scene, -5, -5, w = 10, h = 10)

qfitScene(view)




