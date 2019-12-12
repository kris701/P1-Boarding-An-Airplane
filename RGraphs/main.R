# Install packages
#remove.packages(c("ggplot2", "data.table"))
#install.packages('Rcpp', dependencies = TRUE)
#install.packages('ggplot2', dependencies = TRUE)
#install.packages('data.table', dependencies = TRUE)
#install.packages("tidyverse")
library(tidyverse)
library(ggplot2)
library(dplyr)
library(purrr)

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
    IteMin = min(Iterations)
  );

displayData$boardingName <- factor(displayData$boarding.method, 
                                   levels = c(
                                                "BoardingMethods/backtofront.txt",
                                                "BoardingMethods/backtofront1door.txt"
                                              ),
                                   labels = c(
                                     "Back to Front",
                                     "Back to Front ( 1 door )"
                                     )
)

for(iteratedMethod in unique(displayData$boarding.method)) {
  
  limitedData = displayData %>% subset(boarding.method == iteratedMethod)
  
  generatedPlot = limitedData %>% ggplot(aes(x = luggage.count.0, y=IteMean)) + 
    geom_point() + geom_line() +
    geom_ribbon(aes(x=luggage.count.0, ymax=IteMax, ymin=IteMin), alpha = 0.6, fill = "skyblue") +
    facet_wrap(~walk.speed.0) +
    labs(title = limitedData$boardingName, x="Luggage Count", y="Average Iterations")
  
  
  ggsave(paste(iteratedMethod,".pdf"), plot = generatedPlot, device = "pdf", path = "plots",
         scale = 1, width = 20, height = 10, units = "cm",
         dpi = 300, limitsize = TRUE)
}

