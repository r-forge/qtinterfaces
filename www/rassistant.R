
sanitize <- function(s)
{
    s <- gsub("\"", "&quot;", s, fixed = TRUE)
    s <- gsub("<", "&lt;", s, fixed = TRUE)
    s <- gsub(">", "&gt;", s, fixed = TRUE)
    s <- gsub("&", "&amp;", s, fixed = TRUE)
    s
}

readPackageHelpMeta <- function(pkg, lib.loc = .libPaths()[1])
{
    hsearch.path <- file.path(lib.loc, pkg, "Meta", "hsearch.rds")
    if (file.exists(hsearch.path))
    {
        x <- .readRDS(hsearch.path)
        if (any(sapply(x, length) > 0)) x
        else NULL
    }
    else NULL
}

readPackageHelpHtml <- function(pkg, lib.loc = .libPaths()[1])
{
    html.path <- file.path(lib.loc, pkg, "html")
    if (file.exists(html.path)) 
        list.files(html.path)
    else character(0)
}

readPackageHelpDoc <- function(pkg, lib.loc = .libPaths()[1])
{
    ## anythig in pkg/doc/ (vignettes etc.), as they are linked to from package 00Index.html files
    doc.path <- file.path(lib.loc, pkg, "doc")
    if (file.exists(doc.path)) 
        list.files(doc.path)
    else character(0)
}

keywordList <- function(meta.info)
{
    stopifnot(length(meta.info) == 4)
    names(meta.info) <- c("files", "aliases", "keywords", "concepts")
    meta.info <- lapply(meta.info, as.data.frame)
    ans <- vector(mode = "list", length = nrow(meta.info$files))
    names(ans) <- as.character(meta.info$files$ID)
    for (id in as.character(meta.info$files$ID))
    {
        ans[[ id ]] <-
            list(title = with(meta.info, as.character(subset(files, ID == id)$title)),
                 keywords = with(meta.info,
                                 unique(c(as.character(subset(files, ID == id)$topic),
                                          as.character(subset(aliases, ID == id)$Aliases),
                                          as.character(subset(keywords, ID == id)$Keywords)))))
        ## will not record concepts as they will be found by text
        ## search (even keywords is not particularly useful, as they
        ## are from the weird predefined list of keyword tokens that
        ## are not meant to be search keywords)
    }
    names(ans) <- as.character(meta.info$files$name)
    ans
}

processPackageHelp <- function(pkg, lib.loc = .libPaths()[1], prefix = lib.loc)
{
    hmeta <- readPackageHelpMeta(pkg, lib.loc)
    if (is.null(hmeta)) return (NULL)
    meta.summary <- keywordList(hmeta)
    names(meta.summary) <- sprintf("%s.html", names(meta.summary))
    html.files <- readPackageHelpHtml(pkg, lib.loc)
    doc.files <- readPackageHelpDoc(pkg, lib.loc)
    meta.summary <- meta.summary[names(meta.summary) %in% html.files]
    if (length(meta.summary) == 0)
    {
        ## warning("No HTML help in package ", pkg)
        return(NULL)
    }
    list(package = pkg, 
         pkg.loc = file.path(prefix, pkg),
         html.files = html.files,
         doc.files = doc.files,
         meta.summary = meta.summary)
}

## http://doc.trolltech.com/4.5/qthelpproject.html

constructFilterAttributes <- function(...)
{
    x <- as.character(c(...))
    if (length(x) == 0) x <- c("rassistant", "1.0")
    sprintf("      <filterAttribute>%s</filterAttribute>", x)
}


constructTitleEntries <- function(x)
{
    with(x,
         c(sprintf("        <section title=\"%s\" ref=\"%s\">",
                   package, file.path(pkg.loc, "html", "00Index.html")),
           sapply(names(meta.summary),
                  function(f) {
                      sprintf("          <section title=\"%s\" ref=\"%s\"/>",
                              sanitize(meta.summary[[f]]$title),
                              file.path(pkg.loc, "html", f))
                  }, USE.NAMES = FALSE),
           "        </section>"))
}

constructKeywordEntries <- function(x)
{
    with(x,
         unlist(lapply(names(meta.summary),
                       function(f) {
                           sprintf("        <keyword name=\"%s\" ref=\"%s\"/>",
                                   sanitize(meta.summary[[f]]$keywords),
                                   file.path(pkg.loc, "html", f))
                       })))
}

constructFileEntries <- function(x)
{
    with(x,
         sprintf("        <file>%s</file>",
                 c(file.path(pkg.loc, "html", html.files),
                   file.path(pkg.loc, "doc", doc.files))))
}

## need to include location of R.css

processLibPath <- function(lib.loc = .libPaths()[1], prefix = lib.loc)
{
    pkgs <- list.files(lib.loc)
    has.html <- file.exists(file.path(lib.loc, pkgs, "html"))
    if (any(has.html))
    {
        css.file <- file.path(lib.loc, "R.css")
        css.list <-
            if (file.exists(css.file))
                list(titles = character(0),
                     keywords = character(0),
                     files = sprintf("        <file>%s</file>", file.path(prefix, "R.css")))
            else NULL
        empty.list <-
            list(titles = character(0),
                 keywords = character(0),
                 files = character(0))
        c(list(css.list),
          lapply(pkgs[has.html],
                 function(pkg) {
                     message(file.path(lib.loc, pkg))
                     pkg.help.info <- processPackageHelp(pkg, lib.loc, prefix)
                     if (is.null(pkg.help.info)) empty.list
                     else
                         list(titles = constructTitleEntries(pkg.help.info),
                              keywords = constructKeywordEntries(pkg.help.info),
                              files = constructFileEntries(pkg.help.info))
                 }))
    }
    else list()
}

processLibraries <- function(lib.paths = .libPaths(), ...)
{
    do.call(c, lapply(lib.paths, processLibPath, ...))
}

writeQHP <- function(x, file = "")
{
    con <-
        if (nzchar(file)) file(file, open = "w")
        else stdout()
    myWriteLines <- function(x) writeLines(x, con = con)
    xml.header <- "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
  <QtHelpProject version=\"1.0\">
    <namespace>org.r-project.r</namespace>
    <virtualFolder>doc</virtualFolder>
    <filterSection>
"
    xml.footer <- "    </filterSection>
  </QtHelpProject>
"
    myWriteLines(xml.header)
    myWriteLines(constructFilterAttributes("rassistant", "1.0"))
    ## Title Sections
    myWriteLines("      <toc>")
    lapply(x, function(pinfo) myWriteLines(pinfo$titles))
    myWriteLines("      </toc>")
    ## Keywords
    myWriteLines("      <keywords>")
    lapply(x, function(pinfo) myWriteLines(pinfo$keywords))
    myWriteLines("      </keywords>")
    ## Files
    myWriteLines("      <files>")
    lapply(x, function(pinfo) myWriteLines(pinfo$files))

    ## FIXME: Some special files.  This code is valid for UNIX.  I
    ## will not try to factor this out until I figure out what happens
    ## on Windows.
    addFilesInDir <- function(...)
    {
        loc <- file.path(tempdir(), ...)
        myWriteLines(sprintf("        <file>%s</file>", file.path(..., list.files(loc))))
    }
    addFilesInDir(".R", "doc")
    addFilesInDir(".R", "doc", "html")
    addFilesInDir(".R", "doc", "manual")
    addFilesInDir(".R", "doc", "html", "search")

    myWriteLines("      </files>")
    myWriteLines(xml.footer)
    if (nzchar(file)) close(con)
}

generateCompiledHelpUnix <- function(x, qchfile = "~/rproject.qch")
{
    if (!file.exists(file.path(tempdir(), ".R", "doc", "html", "packages.html")))
        make.packages.html()
    base <- tempdir()
    qhpfile <- file.path(base, "rproject.qhp")
    prefix <- file.path(".R", "library")
    if (missing(x))
    {
        lib.loc <- file.path(base, ".R", "library")
        x <- processLibraries(lib.loc, prefix)
    }
    qhpfile <- path.expand(qhpfile)
    writeQHP(x, file = qhpfile)
    qchfile <- path.expand(qchfile)
    system(sprintf("qhelpgenerator %s -o %s", qhpfile, qchfile))
    qchfile
}

registerHelp <-
    function(qchfile = "~/rproject.qch",
             start.assistant = FALSE)
{
    qchfile <- path.expand(qchfile)
    if (file.exists(qchfile))
    {
        system(sprintf("assistant -unregister %s", qchfile))
        system(sprintf("assistant -register %s", qchfile))
        if (start.assistant) 
            system("assistant &")
    }
    else warning("File ", qchfile, " not found")
}



generateCompiledCollection <-
    function(qchfile = "rproject.qch",
             qhcpfile = "~/rproject.qhcp",
             qhcfile = "~/rproject.qhc",
             start.assistant = FALSE)
{
    qchfile <- path.expand(qchfile)
    qhcpfile  <- path.expand(qhcpfile)
    qhcfile <- path.expand(qhcfile)

    template <- "<?xml version=\"1.0\" encoding=\"utf-8\" ?>
 <QHelpCollectionProject version=\"1.0\">
     <assistant>
         <title>R Help</title>
         <startPage>qthelp://org.r-project.r/doc/.R/doc/html/index.html</startPage>
         <currentFilter>rassistant</currentFilter>
         <enableFilterFunctionality visible=\"true\">true</enableFilterFunctionality>
         <enableDocumentationManager>false</enableDocumentationManager>
         <enableAddressBar visible=\"false\">false</enableAddressBar>
         <cacheDirectory>rproject/rassistant</cacheDirectory>
     </assistant>
     <docFiles>
         <register>
             <file>%s</file>
         </register>
     </docFiles>
 </QHelpCollectionProject>
"

##          <aboutMenuText>
##              <text>About R</text>
##          </aboutMenuText>
##          <aboutDialog>
##              <file>aboutR.txt</file>
##              <icon>about.png</icon>
##          </aboutDialog>

    
    if (file.exists(file.path(dirname(qhcpfile), qchfile)))
    {
        qhcp.contents <- sprintf(template, qchfile)
        writeLines(qhcp.contents, con = qhcpfile)
        ## To create the binary collection file, run the qcollectiongenerator tool:
        ##  qcollectiongenerator mycollection.qhcp -o mycollection.qhc
        system(sprintf("qcollectiongenerator %s -o %s", qhcpfile, qhcfile))
        ## To test the generated collection file, start Qt Assistant in the following way:
        ##  assistant -collectionFile mycollection.qhc
        if (start.assistant) 
            system(sprintf("assistant -collectionFile %s &", qhcfile))
        qchfile
    }
    else warning("File ", qhcfile, " not found")
}







