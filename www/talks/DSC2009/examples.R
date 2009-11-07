

library(mosaiq)

data(Oats, package = "nlme")

mosaiq.xyplot(yield ~ nitro, data = Oats,
              margin = ~ Block + Variety, type = 'o')










library(lattice)

df <- data.frame(x = rnorm(1000),
                 y = rnorm(1000),
                 a = gl(100, 1, 1000))

x11(type = "Xlib")
system.time(print(xyplot(y ~ x | a, df)))



## Graphics device based on scene graph

library(qtdevice)

rscene <- qsceneDevice(7, 7)

library(lattice)
dotplot(VADeaths, auto.key = TRUE, type = c("p", "l"),
        par.settings = simpleTheme(pch = 16))

foo <- qgraphicsView(rscene)
foo

qsetTransform(foo, scale = 2)
qsetTransform(foo, scale = 1/2)
qsetAntialias(foo, TRUE)

## Tedious to scale by hand; so add "Actions":

zoominAct <-
    qaction(desc = "Zoom In",
            shortcut = "Ctrl++",
            parent = foo)
zoomoutAct <-
    qaction(desc = "Zoom Out",
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

qsetContextMenuPolicy(foo, "actions")
qsetDragMode(foo, "scroll")

## Add another action for printing

printAct <- qaction(desc = "Print", shortcut = "Ctrl+P", parent = foo)
qconnect(printAct, signal = "triggered",
         handler = qrenderGraphicsView,
         user.data = foo)
qaddAction(foo, printAct)


## What else can we do?

library(GOstats)
library(GO.db)
library(Rgraphviz)


g4 <- GOGraph(c("GO:0003680", "GO:0003701"), GOMFPARENTS)
gx <- layoutGraph(g4, layoutType = "dot")
renderGraph(gx)

## Every element in the plot is an element in the scene graph

item.list <- qitems(rscene, rscene$sceneRect)
item.text <- sapply(item.list, qtext)
item.text
which.goid <- which(substring(item.text, 1, 2) == "GO")

constructGOToolTip <- function(id) 
{
    with(toTable(GOTERM[id]),
         sprintf("<html><strong>%s</strong><br/>%s</html>",
                 Term, Definition))
}

for (i in which.goid)
    qsetToolTip(item.list[[i]], constructGOToolTip(item.text[i]))



constructGOHtmlLink <- function(id) 
{
    with(toTable(GOTERM[id]),
         sprintf("<html><a href='http://amigo.geneontology.org/cgi-bin/amigo/term-details.cgi?term=%s'
title='%s'>%s</a></html>",
                 id, Term, id))
}

for (i in which.goid)
    qsetText(item.list[[i]], constructGOHtmlLink(item.text[i]))

qsetDragMode(foo, "none")
qsetTextItemInteraction(rscene, mode = "browser")


## add more items!

r <- rscene$sceneRect
r

qscene.text(rscene, r[1,1] + r[2,1]/2, r[1,2] + 0.01 * r[2,2],
            labels = "A demo of the <a href='http://qtinterfaces.r-forge.r-project.org'>R/Qt Interface</a>",
            html = TRUE)



## mosaiq

library(mosaiq)

export.mosaiq("fig/mosaiq_%03g.png")

data(Chem97, package = "mlmRev")
xtabs( ~ score, data = Chem97)

mosaiq.histogram(gcsescore, data = Chem97,
                 margin = ~factor(score))

mosaiq.densityplot(gcsescore, data = Chem97,
                   margin = ~factor(score))

mosaiq.densityplot(gcsescore, data = Chem97,
                   groups = score,
                   legend.args = list(columns = 3))

data(Oats, package = "nlme")

mosaiq.xyplot(yield ~ nitro, data = Oats,
              margin = ~ Block + Variety, type = 'o')

## multiple pages

mosaiq.xyplot(yield ~ nitro, data = Oats,
              margin = ~ Block + Variety, type = 'o',
              layout = c(3, 2),
              main = "Yield of Oats")

data(barley, package = "lattice")

mosaiq.dotplot(yield, variety, data = barley,
               groups = year, margin = ~ site,
               layout = c(1, 6), aspect = c(0.7))

mosaiq.xyplot(yield, variety,
              data = barley, jitter.y = TRUE)

mosaiq.qqmath(x = gcsescore, data = Chem97,
              margin = ~factor(score),
              f.value = ppoints(100),
              main = "Normal Q-Q plot")

Chem97.mod <-
    transform(Chem97,
              gcsescore.trans = gcsescore^2.34)

mosaiq.qqmath(x = gcsescore.trans, data = Chem97.mod,
              groups = gender,
              margin = ~factor(score),
              f.value = ppoints(100),
              main = "Normal Q-Q plot")

mosaiq.bwplot(factor(score) ~ gcsescore, data = Chem97,
              margin = ~ gender,
              xlab = "Average GCSE Score")

mosaiq.bwplot(gcsescore^2.34 ~ gender, data = Chem97,
              margin = ~ factor(score), as.table = FALSE,
              varwidth = TRUE, layout = c(6, 1),
              main = "Transformed GCSE score")


## back to slow lattice example

system.time(print(mosaiq.xyplot(y ~ x, 
                                margin = ~a,
                                data = df)))

