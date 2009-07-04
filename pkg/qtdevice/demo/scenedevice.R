
## demo of the scene/view-based R graphics device, which gives cheap
## pan and zoom with standard R graphics.


library(qtdevice)

foo <- qgraphicsView(rscene <- qsceneDevice(15, 15))

qsetTransform(foo, scale = 1/2)
qsetAntialias(foo, TRUE)

foo

barchart(VADeaths, groups = FALSE, origin = 0,
         par.settings = list(grid.pars = list(lineheight = 4)))

plot(1:10, xlim = c(0, 8), type = "o", pch = c(16, 3), cex = 3)
abline(0, .5)
abline(0, 2)  ## clipping problem

## depth problem

plot(1:100, pch = 21, cex = 5, bg = "pink")


plot(rnorm(100), rnorm(100), pch = 16, cex = 3, col = "#AA222288")
qupdate(foo)

qsetDragMode(foo, "scroll")
qsetAntialias(foo, TRUE)

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

qconnect(zoominAct,
         signal = "triggered",
         handler = function(x, ...) {
             qsetTransform(x, scale = 1.2)
         },
         user.data = foo)

qconnect(zoomoutAct,
         signal = "triggered",
         handler = function(x) {
             qsetTransform(x, scale = 1/1.2)
         },
         user.data = foo)

## rscene <- attr(foo, "scene")

r <- rscene$sceneRect
r

qscene.text(rscene, r[1,1] + r[2,1]/2, r[1,2] + 0.01 * r[2,2],
            labels = "A demo of the <a href='http://qtinterfaces.r-forge.r-project.org'>R/Qt Interface</a>",
            html = TRUE)



printAct <- qaction(desc = "Print", shortcut = "Ctrl+P", parent = foo)
qconnect(printAct, signal = "triggered",
         handler = function(x, ...) {
             qrenderGraphicsView(x)
         },
         user.data = foo)
qaddAction(foo, printAct)


## qsetItemFlags, qsetTextItemInteraction

qsetDragMode(foo, "select")
qsetItemFlags(rscene, "movable", TRUE)
qsetItemFlags(rscene, "selectable", TRUE)


qsetItemFlags(rscene, "selectable", FALSE)
qsetItemFlags(rscene, "movable", FALSE)

qsetTextItemInteraction(rscene, "editor")
qsetTextItemInteraction(rscene, "browser")
qsetTextItemInteraction(rscene, "none")



if (require(Rgraphviz))
{

    g <- randomGraph(as.character(1:20), M = 1:4, 0.4)
    x <- layoutGraph(g, layoutType = "neato")
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




