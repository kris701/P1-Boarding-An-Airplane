# Install packages
#remove.packages(c("ggplot2", "data.table"))
#install.packages('Rcpp', dependencies = TRUE)
#install.packages('ggplot2', dependencies = TRUE)
#install.packages('data.table', dependencies = TRUE)


backToFront = read.csv('../P1-Boarding-An-Airplane/P1-Boarding-An-Airplane/Outputs/backtofront.csv')

print(backToFront)

TEST <- backToFront




# https://stackoverflow.com/questions/25244241/line-plot-with-average-and-shadow-for-min-max

TEST <- transform(TEST, Min = pmin(backToFront$Iterations), Max = pmax(backToFront$Iterations), indx = seq_len(dim(TEST)[1]))
library(ggplot2)
ggplot(TEST) +
  geom_line(aes(x = backToFront$walk.speed.0, y=mean(backToFront$Iterations)), group = 1) +
  geom_ribbon(aes(x = backToFront$walk.speed.0, ymax = Max, ymin = Min), alpha = 0.6, fill = "skyblue")


