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
library(extrafont)

data = read.csv("data/output.csv");

displayData = data %>% 
  group_by(
    boarding.method,
    luggage.count.0,
    walk.speed.0
  ) %>% 
  summarise(
    IteMeanAssignedDoor = mean(Iterations[assign.to.nearest.door==1]),
    IteMaxAssignedDoor  = max( Iterations[assign.to.nearest.door==1]),
    IteMinAssignedDoor  = min( Iterations[assign.to.nearest.door==1]),
    
    IteMeanUnassignedDoor = mean(Iterations[assign.to.nearest.door==0]),
    IteMaxUnassignedDoor  = max( Iterations[assign.to.nearest.door==0]),
    IteMinUnassignedDoor  = min( Iterations[assign.to.nearest.door==0])
);

displayData$boardingName <- factor(displayData$boarding.method, 
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

displayData$walkLabel <- sprintf( "%s walking speed", displayData$walk.speed.0 )

yMax = max(data$Iterations);
yMin = min(data$Iterations);

limitedData = displayData %>% subset(boarding.method == "BoardingMethods/backtofront.txt")
limitedData %>% ggplot() +
  ylim(yMin, yMax) +
  geom_ribbon(aes(x=luggage.count.0, ymax=IteMaxAssignedDoor  , ymin=IteMinAssignedDoor),   alpha = 0.5, fill = "blue") +
  geom_ribbon(aes(x=luggage.count.0, ymax=IteMaxUnassignedDoor, ymin=IteMinUnassignedDoor), alpha = 0.5, fill = "orange") +
  
  geom_line(aes(x = luggage.count.0, y=IteMeanAssignedDoor), color="blue") +
  geom_point(aes(x = luggage.count.0, y=IteMeanAssignedDoor), color="blue") +

  geom_line(aes(x = luggage.count.0, y=IteMeanUnassignedDoor), color="red") +
  geom_point(aes(x = luggage.count.0, y=IteMeanUnassignedDoor), color="red") +

  facet_wrap(~walkLabel) +
  labs(title = limitedData$boardingName, x="Luggage Count", y="Average Iterations")



for(iteratedMethod in unique(displayData$boarding.method)) {
  
  limitedData = displayData %>% subset(boarding.method == iteratedMethod)
  
  generatedPlot = limitedData %>% ggplot() +
    ylim(yMin, yMax) +
    geom_ribbon(aes(x=luggage.count.0, ymax=IteMaxAssignedDoor  , ymin=IteMinAssignedDoor),   alpha = 0.5, fill = "blue") +
    geom_ribbon(aes(x=luggage.count.0, ymax=IteMaxUnassignedDoor, ymin=IteMinUnassignedDoor), alpha = 0.5, fill = "orange") +
    
    geom_line(aes(x = luggage.count.0, y=IteMeanAssignedDoor), color="blue") +
    geom_point(aes(x = luggage.count.0, y=IteMeanAssignedDoor), color="blue") +
    
    geom_line(aes(x = luggage.count.0, y=IteMeanUnassignedDoor), color="red") +
    geom_point(aes(x = luggage.count.0, y=IteMeanUnassignedDoor), color="red") +
    
    facet_wrap(~walkLabel) +
    labs(title = limitedData$boardingName, x="Luggage Count", y="Average Iterations")
  
  ggsave(paste(iteratedMethod,".pdf", sep = ""), plot = generatedPlot, device = "pdf", path = "plots",
         scale = 1, width = 20, height = 10, units = "cm",
         dpi = 300, limitsize = TRUE)
}

