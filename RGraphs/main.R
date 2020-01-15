# Install packages
#remove.packages(c("ggplot2", "data.table"))
#install.packages('Rcpp', dependencies = TRUE)
#install.packages('ggplot2', dependencies = TRUE)
#install.packages('data.table', dependencies = TRUE)
#install.packages("tidyverse")
#install.packages("extrafont")
library(tidyverse)
library(ggplot2)
library(dplyr)
library(purrr)
#library(extrafont)

data = read.csv("data/output.csv");

data$boardingName <- factor(data$boarding.method, 
   levels = c( 
      "backtofrontmirrored", 
      "backtofront", 
      "backtofront1door", 
      "fronttobackmirrored", 
      "fronttoback", 
      "fronttoback1door", 
      "randommirrored", 
      "random", 
      "random1door", 
      "steffenmodifiedmirrored", 
      "steffenmodified", 
      "steffenmodified1door", 
      "steffenperfectmirrored", 
      "steffenperfect", 
      "steffenperfect1door", 
      "wilmamirrored",
      "wilma",
      "wilma1door"
   ),
   
   labels = c( 
      "Back to Front ( mirrored )", 
      "Back to Front ( 2 doors )", 
      "Back to Front ( 1 door )", 
      "Front to back ( mirrored )", 
      "Front to back ( 2 doors )", 
      "Front to back ( 1 door )", 
      "Random ( mirrored )", 
      "Random ( 2 doors )", 
      "Random ( 1 door )", 
      "Steffen Modified ( mirrored )", 
      "Steffen Modified ( 2 doors )", 
      "Steffen Modified ( 1 door )", 
      "Steffen Perfect ( mirrored )", 
      "Steffen Perfect ( 2 doors )", 
      "Steffen Perfect ( 1 door )", 
      "Wilma ( mirrored )",
      "Wilma ( 2 doors )",
      "Wilma ( 1 door )"
   )
);
data$methodGroup <- factor(data$boarding.method, 
                            levels = c( 
                               "backtofrontmirrored", 
                               "backtofront", 
                               "backtofront1door", 
                               "fronttobackmirrored", 
                               "fronttoback", 
                               "fronttoback1door", 
                               "randommirrored", 
                               "random", 
                               "random1door", 
                               "steffenmodifiedmirrored", 
                               "steffenmodified", 
                               "steffenmodified1door", 
                               "steffenperfectmirrored", 
                               "steffenperfect", 
                               "steffenperfect1door", 
                               "wilmamirrored",
                               "wilma",
                               "wilma1door"
                            ),
                            
                            labels = c( 
                               "Back to Front", 
                               "Back to Front", 
                               "Back to Front", 
                               "Front to back", 
                               "Front to back", 
                               "Front to back", 
                               "Random", 
                               "Random", 
                               "Random", 
                               "Steffen Modified", 
                               "Steffen Modified", 
                               "Steffen Modified", 
                               "Steffen Perfect", 
                               "Steffen Perfect", 
                               "Steffen Perfect", 
                               "Wilma",
                               "Wilma",
                               "Wilma"
                            )
);
data$doorType <- factor(data$boarding.method, 
                           levels = c( 
                              "backtofrontmirrored", 
                              "backtofront", 
                              "backtofront1door", 
                              "fronttobackmirrored", 
                              "fronttoback", 
                              "fronttoback1door", 
                              "randommirrored", 
                              "random", 
                              "random1door", 
                              "steffenmodifiedmirrored", 
                              "steffenmodified", 
                              "steffenmodified1door", 
                              "steffenperfectmirrored", 
                              "steffenperfect", 
                              "steffenperfect1door", 
                              "wilmamirrored",
                              "wilma",
                              "wilma1door"
                           ),
                           
                           labels = c( 
                              "mirrored", 
                              "2 doors", 
                              "1 door", 
                              "mirrored", 
                              "2 doors", 
                              "1 door", 
                              "mirrored", 
                              "2 doors", 
                              "1 door", 
                              "mirrored", 
                              "2 doors", 
                              "1 door", 
                              "mirrored", 
                              "2 doors", 
                              "1 door", 
                              "mirrored", 
                              "2 doors", 
                              "1 door"
                           )
);


isSingleDoor = function(methodName) {
   return(grepl(methodName, "1"))
}


yMax = max(data$Iterations);
yMin = min(data$Iterations);

data = data[data$assign.to.nearest.door == 0 || data$assign.to.nearest.door == 1]

print("Loaded Main.R");






