
if (FALSE)
{
    
    ## doesn't work any more with R 2.10 

    ## source("http://qtinterfaces.r-forge.r-project.org/rassistant.R")
    source("/home/dsarkar/svn/all/r-forge.r-project.org/qtinterfaces/www/rassistant.R")

    generateCompiledHelpUnix() # takes a few minutes to run for many packages

    registerHelp()
    generateCompiledCollection(start.assistant = TRUE)

    ## 1. Navigate using table of contents
    ##
    ## 2. HTML links etc., tabbed browsing
    ##
    ## 3. Indexing allows fast keyword lookup (alias, concept entries)
    ##    - most useful mode: just start typing topic name
    ##    - finds partial matches: e.g. S4 methods
    ##    - handles duplicate hits
    ##
    ## 4. Full text search
}

## Object viewer

library(qtutils)

## 'Show' objects
qstr(plot)
qstr(cars)
qstr(as.environment("package:datasets"))

## Object browser (qstr() on list of search() entries)
qbrowser()

## Graphical replacement for recover()
options(error = qrecover)
n <- 100
df <- data.frame(x = seq_len(n), y = rnorm(n), w = runif(n)-0.1)
with(df, density(y, weights = w))

options(error = NULL)

## Data import

## unicode <- "/usr/share/perl/5.10.0/unicore/UnicodeData.txt"
## system(sprintf("cp %s /tmp/", unicode))
## unidata <- data.import() # stack imbalance


## Graphics device based on scene graph

## library(qtdevice)

rscene <- qsceneDevice(7, 7)

library(lattice)
dotplot(VADeaths, auto.key = TRUE, type = c("p", "l"),
        par.settings = simpleTheme(pch = 16))

gview <- Qt$QGraphicsView(rscene)
gview

gview$scale(2, 2)
gview$scale(1/2, 1/2)

## gview$translate(100, 100) #??

gview$setRenderHints(Qt$QPainter$Antialiasing) 
gview$setDragMode(1L) ## ?? Qt$QGraphicsView$ScrollHandDrag


## print view

printer <- Qt$QPrinter(Qt$QPrinter$HighResolution)
printer$setPageSize(Qt$QPrinter$A4)
printer$setOutputFileName("/tmp/qt.pdf")

painter <- Qt$QPainter()
painter$begin(printer)
## to print view: gview$render(painter)
rscene$render(painter)
painter$end()


## Tedious to scale by hand; so add "Actions":

zoominAct <- Qt$QAction("Zoom In", gview)
zoominAct$setShortcut(Qt$QKeySequence("Ctrl++"))
qconnect(zoominAct,
         signal = "triggered",
         handler = function(checked) {
             gview$scale(1.2, 1.2)
         })
gview$addAction(zoominAct)

zoomoutAct <- Qt$QAction("Zoom Out", gview)
zoomoutAct$setShortcut(Qt$QKeySequence("Ctrl+-"))
qconnect(zoomoutAct,
         signal = "triggered",
         handler = function(checked) {
             gview$scale(1/1.2, 1/1.2)
         })
gview$addAction(zoomoutAct)

gview$setContextMenuPolicy(Qt$Qt$ActionsContextMenu)



## What else can we do?

sc <- gview$scene()
items <- sc$items()

itext <- 
lapply(items,
       function(x) {
           text <- try(x$toPlainText(), silent = TRUE)
           text
       })
       

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


## add more items!

r <- rscene$sceneRect
r

qscene.text(rscene, r[1,1] + r[2,1]/2, r[1,2] + 0.01 * r[2,2],
            labels = "A demo of the <a href='http://qtinterfaces.r-forge.r-project.org'>R/Qt Interface</a>",
            html = TRUE)

## mosaiq

library(mosaiq)
data(Chem97, package = "mlmRev")
xtabs( ~ score, data = Chem97)

mosaiq.histogram(gcsescore, data = Chem97, margin = ~factor(score))

mosaiq.densityplot(gcsescore, data = Chem97, margin = ~factor(score))

mosaiq.densityplot(gcsescore, data = Chem97, groups = score,
                   legend.args = list(columns = 3))


mosaiq.qqmath(x = gcsescore, data = Chem97,
              margin = ~factor(score),
              f.value = ppoints(100))

library("latticeExtra")
data(gvhd10)

mosaiq.densityplot(log(FSC.H), data = gvhd10, margin = ~Days, layout = c(2, 4))

mosaiq.histogram(~log2(FSC.H), data = gvhd10, margin = ~Days,
                 xlab = "log Forward Scatter",
                 type = "density", nint = 50,
                 layout = c(2, 4))


## x11(type = "Xlib")

## library(lattice)
## histogram(~log2(FSC.H) | Days, data = gvhd10,
##           xlab = "log Forward Scatter",
##           type = "density", nint = 50,
##           layout = c(2, 4))




## dynamic example

source("/home/dsarkar/qtpaint-demos/tourr-gui.r")

gui_xy(diamonds)
gui_xy(flea)
