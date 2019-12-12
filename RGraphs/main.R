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

displayData <- data %>% 
  group_by(
    boarding.method,
    luggage.count.0,
    walk.speed.0,
    ) %>% 
  summarise(
    IteMean = mean(Iterations), 
    IteMax = max(Iterations), 
    IteMin = min(Iterations),
#    WalkLabel = paste(walk.speed.0, " walking speed")
  );

displayData$boardingName <- factor(displayData$boarding.method, 
   levels = c( 
     "BoardingMethods/backtofront.txt", 
     "BoardingMethods/backtofront1door.txt", 
     "BoardingMethods/fronttoback.txt", 
     "BoardingMethods/fronttoback1door.txt", 
     "BoardingMethods/random.txt", 
     "BoardingMethods/random1door.txt", 
     "BoardingMethods/steffenmodified.txt", 
     "BoardingMethods/steffenmodified1door.txt", 
     "BoardingMethods/steffenperfect.txt", 
     "BoardingMethods/steffenperfect1door.txt", 
     "BoardingMethods/wilma.txt",
     "BoardingMethods/wilma1door.txt"
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

displayData$walkLabel = factor(displayData$walk.speed.0,
                               levels = c(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29),
                               label  = c("0 walking speed", "1 walking speed", "2 walking speed", "3 walking speed", "4 walking speed", "5 walking speed", "6 walking speed", "7 walking speed", "8 walking speed", "9 walking speed", "10 walking speed", "11 walking speed", "12 walking speed", "13 walking speed", "14 walking speed", "15 walking speed", "16 walking speed", "17 walking speed", "18 walking speed", "19 walking speed", "20 walking speed", "21 walking speed", "22 walking speed", "23 walking speed", "24 walking speed", "25 walking speed", "26 walking speed", "27 walking speed", "28 walking speed", "29 walking speed")
)

displayData$walkLabel <- sprintf( "%s walking speed", displayData$walk.speed.0 )


yMax = max(data$Iterations);
yMin = min(data$Iterations);


limitedData = displayData %>% subset(boarding.method == "BoardingMethods/backtofront.txt")
limitedData %>% ggplot(aes(x = luggage.count.0, y=IteMean)) +
  ylim(yMin, yMax) +
  geom_point() + geom_line() +
  geom_ribbon(aes(x=luggage.count.0, ymax=IteMax, ymin=IteMin), alpha = 0.6, fill = "skyblue") +
  facet_wrap(~walkLabel) +
  labs(title = limitedData$boardingName, x="Luggage Count", y="Average Iterations")


for(iteratedMethod in unique(displayData$boarding.method)) {
  
  limitedData = displayData %>% subset(boarding.method == iteratedMethod)
  
  generatedPlot = limitedData %>% ggplot(aes(x = luggage.count.0, y=IteMean)) +
    ylim(yMin, yMax) +
    geom_point() + geom_line() +
    geom_ribbon(aes(x=luggage.count.0, ymax=IteMax, ymin=IteMin), alpha = 0.6, fill = "skyblue") +
    facet_wrap(~walkLabel) +
    labs(title = limitedData$boardingName, x="Luggage Count", y="Average Iterations")
  
  
  ggsave(paste(iteratedMethod,".pdf"), plot = generatedPlot, device = "pdf", path = "plots",
         scale = 1, width = 20, height = 10, units = "cm",
         dpi = 300, limitsize = TRUE)
}

