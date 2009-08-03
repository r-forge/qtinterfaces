
getInfo <- function(file)
{
    ll <- readLines(file)
    comments <- grep("^[[:space:]]*//", ll)
    if (length(comments) > 0)
        ll <- ll[-comments]
    qtfunloc <- grep("qt_", ll, fixed = TRUE)
    n <- length(qtfunloc)
    defs <- 
        sapply(seq_len(n), 
               function(i) {
                   start <- qtfunloc[i]
                   end <- if (i == n) length(ll) else (qtfunloc[i + 1] - 1)
                   paste(ll[start:end], collapse = " ")
               })
    paren <- regexpr(")[[:space:]]*\\{", defs)
    invalid <- paren < 0
    if (any(invalid)) {
        print(defs[invalid])
        ## stop("close-paren not found")
        defs <- defs[!invalid]
        paren <- paren[!invalid]
    }
    defs <- gsub("SEXP ", "", substring(defs, 1, paren), fixed = TRUE)
    exprs <- parse(text = defs)
    exprs
}

generateCallDefs <- function(exprs)
{
    lens <- sapply(exprs, length) - 1L
    nms <- as.character(lapply(exprs, "[[", 1))
    paste(sprintf("    CALLDEF(%s, %d),", nms, lens), "\n", sep = "", collapse = "")
}

generateHeader <- function(exprs, pkg, file,
                           var = paste(toupper(pkg), toupper(gsub(".cpp", "", file, fixed = TRUE)), "H", sep = "_"))
{
    lens <- sapply(exprs, length) - 1L
    nms <- as.character(lapply(exprs, "[[", 1))
    args <-
        sapply(lens,
               function(i) {
                   paste(rep("SEXP", i), collapse = ",")
               })
    decls <- sprintf("SEXP %s(%s);", nms, args)
    c(sprintf("#ifndef %s", var), sprintf("#define %s", var), "", decls, "", "#endif", "")
}



## generate S3 method declarations

generateNamespaceEntries <- function(exprs)
{
    nms <- as.character(lapply(exprs, "[[", 1))
    sapply(strsplit(nms, "_"),
           function(x) {
               if (length(x) == 2)
                   sprintf("# export(%s)", x[2])
               else if (length(x) == 3)
                   sprintf("S3method(%s, %s)", x[2], x[3])
               else
                   stop("Malformatted function name")
           })
}

processPackage <- function(pkg, files = list.files(file.path(pkg, "src"), pattern = "cpp$"))
{
    calldefs <- character(0)
    nsdecs <- character(0)
    for (f in files)
    {
        message(f)
        exprs <- getInfo(file.path(pkg, "src", f))
        cat(paste(generateHeader(exprs, pkg = pkg, file = f), "\n", sep = "", collapse = ""),
            sep = "",
            file = file.path(pkg, "src", gsub(".cpp", ".h", f, fixed = TRUE)))
        calldefs <- c(calldefs, generateCallDefs(exprs))
        nsdecs <- c(nsdecs, generateNamespaceEntries(exprs))
    }
    cat(paste(calldefs, "\n", sep = "", collapse = ""),
        sep = "",
        file = file.path(pkg, "src", "calldefs.inc"))
    cat(paste(nsdecs, "\n", sep = "", collapse = ""),
        sep = "",
        file = file.path(pkg, "NAMESPACE.inc"))
    invisible()
}


processPackage("qtutils", file = c("dataview.cpp", "editor.cpp", "listview.cpp"))

processPackage("qtgui",
               file = c("actions.cpp", "attributes.cpp", "basic.cpp",
                        "containers.cpp", "fonts.cpp", "layout.cpp", "qwidgetWrappers.cpp"))


