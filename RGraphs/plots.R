if(is.null(data)) {
  source(file="main.R")
}

displayData = data %>% 
  group_by(
    boarding.method,
    boardingName,
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

displayData$walkLabel <- sprintf( "%s walking speed", displayData$walk.speed.0 )


yMax = max(data$Iterations);
yMin = min(data$Iterations);

limitedData = displayData %>% subset(boarding.method == "backtofront")
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


### All seperate methods
# Fix x-axis


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


