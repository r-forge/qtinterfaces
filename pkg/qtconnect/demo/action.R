
library(qtmisc)

foo <- qeditor("/tmp/foo.R")

act <- qaction("Run R code", shortcut = "Ctrl+R", parent = foo)
qaddAction(foo, act)
qsetContextMenuPolicy(foo, "actions")

foo

bar <-
    qconnect(act, user.data = foo,
             handler = function(x) {
                 print("action triggered")
             }, which = "triggered_bool")




