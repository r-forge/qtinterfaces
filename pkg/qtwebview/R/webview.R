

## a browser widget

qwebView <- 
    function(url = "http://www.r-project.org")
{
    .Call(qt_qwebView, as.character(url[1]))
}

qsetUrl <-
    function(x, url)
{
    if (is(x, "QWebView"))
        .Call(qt_qsetUrl, x, as.character(url[1]))
    invisible()
}

qurl <-
    function(x, url)
{
    if (is(x, "QWebView")) .Call(qt_qurl, x)
    else error("'x' is not a QWebView object")
}

