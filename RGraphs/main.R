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

data = read.csv("data/outputfile.csv");

displayData <- data %>% 
  group_by(
    boarding.method,
    luggage.count.0,
    walk.speed.0
    ) %>% 
  summarise(
    IteMean = mean(Iterations), 
    IteMax = max(Iterations), 
    IteMin = min(Iterations)
  );

displayData %>% ggplot(aes(x = luggage.count.0, y=IteMean, color = factor(boarding.method))) + 
  geom_point() + geom_line() +
  geom_ribbon(aes(x=luggage.count.0, ymax=IteMax, ymin=IteMin), alpha = 0.6, fill = "skyblue") +
  facet_wrap(~walk.speed.0)






