source(file="main.R")






data %>% ggplot(aes(x = boardingName, y = Iterations, color = factor(boardingName))) + 
  geom_boxplot()




print("Done")






















displayData = data %>% 
  group_by(
    boarding.method,
    boardingName,
    luggage.count.0,
    walk.speed.0,
    assign.to.nearest.door
  ) %>% 
  summarise(
    Iterations = Iterations,
    Min = min(iterations),
    Max = max(iterations),
    median
  );


## Boxplot, min, max, median, mean from displayData
#Luggagecount = 2
#Walkingspeed = 2

limitedData = subset(displayData, luggage.count.0 == 2 && walk.speed.0==2)

limitedData %>% ggplot() + 
  geom_boxplot(aes(x = boardingName, y = Iterations, color = factor(boardingName)), position = position_dodge(width = 0.8))# + 
geom_boxplot(position = position_dodge(width = 0.8)) + 
  geom_point(data = d_means, aes(y = Mean, group = factor(boardingName)), 
             shape = 4,
             size = 4,
             position = position_dodge(width = 0.8)) +
  scale_color_discrete(NULL)