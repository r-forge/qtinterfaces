
library(qtutils)

## 'Show' objects
qstr(plot)
qstr(cars)
qstr(as.environment("package:datasets"))

## Object browser (qstr() on list of search() entries)
qbrowser()

## A graphical command-line

qrepl()

## Graphical replacement for recover().  Right click on environment
## listings to start a qrepl() to evaluate compound expressions.

options(error = qrecover)
n <- 100
df <- data.frame(x = seq_len(n), y = rnorm(n), w = runif(n)-0.1)
with(df, density(y, weights = w))

options(error = NULL)


## Graphics device based on scene graph

## library(qtdevice)

rscene <- qsceneDevice(12, 12)

library(lattice)
dotplot(VADeaths, auto.key = TRUE, type = c("p", "l"),
        par.settings = simpleTheme(pch = 16))

gview <- Qt$QGraphicsView(rscene)
gview

gview$setRenderHints(Qt$QPainter$Antialiasing) 
gview$setDragMode(1L) ## ?? Qt$QGraphicsView$ScrollHandDrag


gview$scale(2, 2)
gview$scale(1/2, 1/2)

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


## print scene to PDF file

## first, set up printer
if (TRUE)
{
    printer <- Qt$QPrinter(Qt$QPrinter$HighResolution)
    printer$setPageSize(Qt$QPrinter$A4)
    printer$setOutputFileName("/tmp/qt.pdf")
    acceptPrint <- TRUE
} else ## choose interactively
{
    pd <- Qt$QPrintDialog(printer)
    acceptPrint <- pd$exec()
}

if (acceptPrint)
{
    painter <- Qt$QPainter()
    painter$begin(printer)
    ## to print view: gview$render(painter)
    rscene$render(painter)
    painter$end()
}

## Can also add action to Print using context menu


## What else can we do?

## Every element in the plot is an element in the scene graph.
## Extract text from all text items

items <- rscene$items()
itext <- 
    lapply(items,
           function(x) {
               if (is(x, "QGraphicsSimpleTextItem")) x$text()
               else NA_character_
           })
unlist(itext)

## Something a bit more useful

library(GOstats)
library(GO.db)
library(Rgraphviz)

g4 <- GOGraph(c("GO:0003680", "GO:0003701"), GOMFPARENTS)
gx <- layoutGraph(g4, layoutType = "dot")
renderGraph(gx)

## Find the text items that are GO category names

items <- rscene$items()
itext <- 
    sapply(items,
           function(x) {
               if (is(x, "QGraphicsSimpleTextItem")) x$text()
               else NA_character_
           })

which.goid <- which(substring(itext, 1, 2) == "GO")

## Add tooltips with GO category details

constructGOToolTip <- function(id) 
{
    with(toTable(GOTERM[id]),
         sprintf("<html><strong>%s</strong><br/>%s</html>",
                 Term, Definition))
}

for (i in which.goid)
    items[[i]]$setToolTip(constructGOToolTip(itext[i]))

## Even better: replace text with HTML links

constructGOHtmlLink <- function(id) 
{
    with(toTable(GOTERM[id]),
         sprintf("<html><a href='http://amigo.geneontology.org/cgi-bin/amigo/term-details.cgi?term=%s' title='%s'>%s</a></html>",
                 id, Term, id))
}

for (i in which.goid)
{
    newtext <- Qt$QGraphicsTextItem()
    newtext$setHtml(constructGOHtmlLink(itext[i]))
    newtext$setToolTip(constructGOToolTip(itext[i]))
    newtext$setPos(items[[i]]$x(), items[[i]]$y())
    newtext$setFont(items[[i]]$font())
    newtext$setTextInteractionFlags(Qt$Qt$TextBrowserInteraction)
    newtext$setOpenExternalLinks(TRUE)
    rscene$removeItem(items[[i]])
    rscene$addItem(newtext)
}

gview$setDragMode(0)



## ## dynamic example

## source("/home/dsarkar/qtpaint-demos/tourr-gui.r")

## gui_xy(diamonds)
## gui_xy(flea)

## Data import

## unicode <- "/usr/share/perl/5.10.0/unicore/UnicodeData.txt"
## system(sprintf("cp %s /tmp/", unicode))
## unidata <- data.import() # stack imbalance


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

