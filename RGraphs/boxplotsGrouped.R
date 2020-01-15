if(class(data) != "data.frame") {
  source(file="main.R")
}

for( walkSpeed in unique(data$walk.speed.0) ) {
  for( luggageCount in unique(data$luggage.count.0)) {
    for( assignedDoor in unique(data$assign.to.nearest.door)) {
      
      if(is.na(assignedDoor)) { next }
      
      plotId = paste("WalkSpeed_",walkSpeed, "_luggageCount_",luggageCount, "_assignedDoor_", assignedDoor, ".png", sep="")
      plotTitle = paste("Walk Speed: ",walkSpeed, "; Luggage Count: ",luggageCount, sep="")
      
      if(assignedDoor == 1) { plotTitle = paste(plotTitle, "; Door: Nearest", sep=""); }
      else                  { plotTitle = paste(plotTitle, "; Door: Random" , sep=""); }
      
      print(plotId)
      limitedData = data %>% subset(
        walk.speed.0==walkSpeed &
        luggage.count.0==luggageCount & 
        assign.to.nearest.door==assignedDoor
      );
      
      means = limitedData %>%
        group_by(
          boardingName,
          methodGroup,
          doorType
        ) %>%
        summarise(
          meanIterations = mean(Iterations)
        );
      
      yFrame = data %>% subset( walk.speed.0==walkSpeed & luggage.count.0==luggageCount );
      localYMax = max(yFrame$Iterations);
      localYMin = min(yFrame$Iterations);
      
      print(paste("Subset rows:", nrow(limitedData)));
      
      generatedPlot =
        limitedData %>% ggplot(aes(x = methodGroup, y = Iterations, color = factor(doorType))) + 
        ylim(localYMin, localYMax) +
        geom_boxplot()+
        geom_boxplot(outlier.shape = NA)+
        geom_point(data=means, aes(x = methodGroup, y = meanIterations, color = factor(doorType)), 
                   shape = 4,
                   size = 4,
                   position = position_dodge(width = 0.8)) +
        labs(title = plotTitle, y="Iterations") +
        theme(
          #axis.text.x = element_blank(),
          axis.title.x = element_blank(),
          legend.title = element_blank()
        )
      
      ggsave(plotId, plot = generatedPlot, device = "png", path = "plots/boxplots_grouped",
             scale = 1, width = 20, height = 10, units = "cm",
             dpi = 300, limitsize = TRUE)
    }
  }
}



