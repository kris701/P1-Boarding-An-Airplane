if(is.null(data)) {
  source(file="main.R")
}

for( walkSpeed in unique(data$walk.speed.0) ) {
  for( luggageCount in unique(data$luggage.count.0)) {
    for( assignedDoor in unique(data$assign.to.nearest.door)) {
      plotId = paste("WalkSpeed_",walkSpeed, "_luggageCount_",luggageCount, "_assignedDoor_", assignedDoor, ".pdf", sep="")
      plotTitle = paste("Walk Speed: ",walkSpeed, "; Luggage Count: ",luggageCount, sep="")
      
      if(assignedDoor == 1) { plotTitle = paste(plotTitle, "; Door: Nearest", sep=""); }
      else {                  plotTitle = paste(plotTitle, "; Door: Random" , sep=""); }
      
      print(plotId)
      limitedData = data %>% subset(
        walk.speed.0==walkSpeed &
        luggage.count.0==luggageCount & 
        assign.to.nearest.door==assignedDoor
      );
      
      means = limitedData %>%
        group_by(
          boardingName
        ) %>%
        summarise(
          #y = max(limitedData$Iterations) * 0.95,
          #label = element_text(label = paste("Count:", nrow(limitedData$boardingName[limitedData$boardingName==boardingName]))),
          meanIterations = mean(Iterations)
        );
      
      #https://medium.com/@gscheithauer/how-to-add-number-of-observations-to-a-ggplot2-boxplot-b22710f7ef80
      stat_box_data <- function(y, upper_limit = max(limitedData$Iterations) * 1.15) {
        return( 
          data.frame(
            y = 0.95 * upper_limit,
            label = paste('count:\n', length(y), '\n',
                          'mean:\n', round(mean(y), 1), '\n')
          )
        )
      }
      
      
      print(paste("Subset rows:", nrow(limitedData)));
      
      generatedPlot =
        limitedData %>% ggplot(aes(x = boardingName, y = Iterations, color = factor(boardingName))) + 
        geom_boxplot(outlier.shape = NA)+
        geom_point(data=means, aes(y = meanIterations, group = factor(boardingName)), 
                   shape = 4,
                   size = 4,
                   position = position_dodge(width = 0.8)) +
        #stat_summary(
        #  fun.data = stat_box_data, 
        #  geom = "text", 
        #  hjust = 0.5,
        #  vjust = 0.9
        #) + 
        labs(title = plotTitle, y="Iterations") +
        theme(
          axis.text.x = element_blank(),
          axis.title.x = element_blank()
        )
        
        ggsave(plotId, plot = generatedPlot, device = "pdf", path = "plots/boxplots",
               scale = 1, width = 20, height = 10, units = "cm",
               dpi = 300, limitsize = TRUE)
    }
  }
}



