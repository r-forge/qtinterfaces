
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

## library(qtconnect)

qsetContextMenuPolicy(foo, "actions")

zoominAct <- qaction(desc = "Zoom In",
                     shortcut = "Ctrl++",
                     parent = foo)
zoomoutAct <- qaction(desc = "Zoom Out",
                      shortcut = "Ctrl+-",
                      parent = foo)

qaddAction(foo, zoominAct)
qaddAction(foo, zoomoutAct)

zoominHandler <- 
    qconnect(zoominAct,
             signal = "triggered",
             handler = function(x, ...) {
                 qsetTransform(x, scale = 1.2)
             },
             user.data = foo)

zoomoutHandler <- 
    qconnect(zoomoutAct,
             signal = "triggered",
             handler = function(x) {
                 qsetTransform(x, scale = 1/1.2)
             },
             user.data = foo)

rscene <- attr(foo, "scene")

r <- qsceneRect(rscene)
r

qscene.text(rscene, r[1] + r[3]/2, r[2] + 0.01 * r[4],
            labels = "A demo of the <a href='http://qtinterfaces.r-forge.r-project.org'>R/Qt Interface</a>",
            html = TRUE)



printAct <- qaction(desc = "Print", shortcut = "Ctrl+P", parent = foo)
qconnect(printAct, signal = "triggered",
         handler = function(x, ...) {
             qrenderGraphicsView(x)
         },
         user.data = foo)
printhandler <- qaddAction(foo, printAct)

## qsetItemFlags, qsetTextItemInteraction

qsetDragMode(foo, "select")
qsetItemFlags(rscene, "movable", TRUE)

qsetItemFlags(rscene, "selectable", TRUE)


qsetItemFlags(rscene, "selectable", FALSE)
qsetItemFlags(rscene, "movable", FALSE)

## CRASH
## qtgui:::qsetTextItemInteraction(rscene, "editor")



if (require(Rgraphviz))
{

    g <- randomGraph(as.character(1:20), M = 1:4, 0.4)
    x <- layoutGraph(g, layoutType = "dot")
    par(lwd = 0.0)
    renderGraph(x,
                graph.pars = list(nodes = list(lwd = 0.4),
                                  edges = list(lwd = 0.2)))

}

if (require(latticeExtra))
{
    ## zoom is slow (because of antialiasing?)

    library("mapproj")
    data(USCancerRates)
    rng <- with(USCancerRates, 
                range(rate.male, rate.female, finite = TRUE))
    nbreaks <- 50
    breaks <- exp(do.breaks(log(rng), nbreaks))

    mapplot(rownames(USCancerRates) ~ rate.male + rate.female,
            data = USCancerRates, breaks = breaks,
            map = map("county", plot = FALSE, fill = TRUE, 
                      projection = "tetra"),
            scales = list(draw = FALSE), xlab = "",
            lwd = 0.1,
            main = "Average yearly deaths due to cancer per 100000")

}




