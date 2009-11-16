

QT <- function(width = 7, height = 7, pointsize = 12,
               family = "",
               aa.edges = TRUE, aa.text = TRUE,
               bgimage = "", history.size = 5)
{
    .Call(do_newQtDevice,
          as.numeric(width),
          as.numeric(height),
          as.numeric(pointsize),
          as.character(family),
          as.logical(aa.edges),
          as.logical(aa.text),
          as.character(bgimage))
}

qsetScene <- function(rview, rscene)
{
    .Call(qt_qsetScene, rview, rscene)
}

qsceneDevice <-
    function(width = 7, height = 7, pointsize = 12, family = "")
{
    .Call(qt_qsceneDevice,
          as.numeric(width),
          as.numeric(height),
          as.numeric(pointsize),
          as.character(family))
}

qsceneView <- function(x)
{
    .Call(qt_qsceneView, x)
}


## qt.dev.resize <- function(width, height)
## {
##     if (.Device == "QT")
##     {
##         .Call("resizeCurrentDevice",
##               as.double(width), as.double(height),
##               PACKAGE = "qtutils")
##     }
##     else NULL
## }



## QT <- function()
## {
##     .C("do_startdev")
##     return(invisible(TRUE))
## }
