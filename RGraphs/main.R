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
     "backtofront", 
     "backtofront1door", 
     "fronttoback", 
     "fronttoback1door", 
     "random", 
     "random1door", 
     "steffenmodified", 
     "steffenmodified1door", 
     "steffenperfect", 
     "steffenperfect1door", 
     "wilma",
     "wilma1door"
   ),
   
   labels = c( 
     "Back to Front ( 2 doors )", 
     "Back to Front ( 1 door )", 
     "Front to back ( 2 doors )", 
     "Front to back ( 1 door )", 
     "Random ( 2 doors )", 
     "Random ( 1 door )", 
     "Steffen Modified ( 2 doors )", 
     "Steffen Modified ( 1 door )", 
     "Steffen Perfect ( 2 doors )", 
     "Steffen Perfect ( 1 door )", 
     "Wilma ( 2 doors )",
     "Wilma ( 1 door )"
   )
);

isSingleDoor = function(methodName) {
   return(grepl(methodName, "1"))
}


yMax = max(data$Iterations);
yMin = min(data$Iterations);

print("Loaded Main.R");






