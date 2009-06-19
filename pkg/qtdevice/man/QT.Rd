\name{qt}
\alias{QT}
\alias{viewDataFrame}
\title{A QT Graphics Device}
\description{
  An R Graphics device and a data viewer implemeted as Qt widgets
}
\usage{
QT(width=6, height=6, pointsize=10,
   family = "", bgimage = "")
viewDataFrame(data)
}
\arguments{
  \item{width}{the width of the plotting window in inches.}
  \item{height}{the height of the plotting window in inches.}
  \item{pointsize}{the default pointsize to be used.}
  \item{family}{ the default font family, specified as a font family
    name recognized by Qt.  Qt has a sophisticated name matching
    algorithm, so this doesn't have to be exact. }
  \item{bgimage}{the path of an image file (in a format recognized by
    Qt) that will serve as a background }
  \item{data}{ an \R object, coerced to be a \code{data.frame} before
    being displayed.  No editing is supported. }
}

\details{

  The QT R Graphics device is an interactive screen device like X11() on
  UNIX/Linux and windows() on Microsoft Windows. Its primary benefits
  are support for alpha transparency and better handling of X11 events
  (unlike an X11 device, it does not need a redraw at the R level on
  every expose event). Fonts are handled transparently through QT, which
  allows access to a wider selection of fonts and advanced typographical
  features such as OpenType lookups with no extra effort. It is also
  cross-platform (potentially---it doesn't work on Mac OS X yet).

  Most features expected in a screen device are available, including
  locator-like interaction. R line types are not fully supported, and
  plotmath behaviour can be incorrect in some situations.

  A context menu allows printing and saving to external image files.
  Note however that the backend storage is a bitmap image (not a vector
  format), so one should use \code{\link{dev.copy}} to save in vector
  formats like PDF and SVG.

}

\author{Deepayan Sarkar} 
\references{
  \url{http://www.trolltech.com} for information about \code{Qt}.
  \url{http://dsarkar.fhcrc.org/R/R-Qt.html}
}
\seealso{
  \code{\link{x11}},
  \code{\link{Devices}}.
}
\examples{
  QT()
  plot(1:10)
}
\keyword{device}
