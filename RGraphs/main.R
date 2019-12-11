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

#Import Data #https://stackoverflow.com/questions/30054765/how-to-get-r-to-read-in-files-from-multiple-subdirectories-under-one-large-direc
#datafiles = list.files(path="data", recursive=TRUE, full.names=TRUE)
#data = {}
#for(file in datafiles) {
#  data = append(data, read.csv(file));
#}
#data = read.csv("/data/outputfile.csv")
#temp = list.files(pattern="*.csv")
#data = sapply(datafiles, read.delim)


data = read.csv("data/outputfile.csv");

print(data)

groupedData = data %>%
#  unlist() %>%
  group_by(boarding.method, walk.speed.0, luggage.count.0);

groupedData = groupedData %>% transform(
  Min = pmin(Iterations),
  Max = pmax(Iterations),
  #  Mean = sapply(groupedData$Iterations, mean),
  #Mean = mean(groupedData$Iterations),
  graphId = boarding.method
);

groupedData = groupedData[ which(groupedData$boarding.method=="BoardingMethods/steffenmodified.txt"),]
groupedData = groupedData[ which(groupedData$walk.speed.0==1),]
#groupedData = groupedData[ which(groupedData$luggage.count.0),]

Mean = sapply(groupedData$Iterations, mean);

# https://stackoverflow.com/questions/25244241/line-plot-with-average-and-shadow-for-min-max
ggplot(groupedData) +
  geom_line(  aes(x=luggage.count.0, y=Mean)) +
  #geom_ribbon(aes(x=luggage.count.0, ymax=Max, ymin=Min), alpha = 0.6, fill = "skyblue") + 
  geom_ribbon(aes(x=luggage.count.0, ymax=Max, ymin=Min), alpha = 0.6, fill = "red")





backToFront = read.csv('../P1-Boarding-An-Airplane/P1-Boarding-An-Airplane/Outputs/backtofront.csv')
TEST = backToFront %>% group_by(walk.speed.0)
TEST <- TEST %>% transform(
  Min = pmin(Iterations),
  Max = pmax(Iterations),
  indx = seq_len(dim(TEST)[1])
)

Mean = (TEST %>% group_by(walk.speed.0))$Iterations %>% sapply(mean)

# https://stackoverflow.com/questions/25244241/line-plot-with-average-and-shadow-for-min-max
ggplot(TEST) +
  geom_line(aes(x = indx, y=Mean))# +
  geom_ribbon(aes(x = TEST$indx, ymax = Max, ymin = Min), alpha = 0.6, fill = "skyblue")

ggplot(TEST) +
  geom_line(aes(x = TEST$walk.speed.0, y=Mean), group = 1) +
  geom_ribbon(aes(x = TEST$walk.speed.0, ymax = Max, ymin = Min), alpha = 0.6, fill = "skyblue")

print(TEST)

