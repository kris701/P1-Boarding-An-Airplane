source(file="main.R")


means = data %>%
  group_by(boardingName) %>%
  summarise(
    meanIterations = mean(Iterations)
  );



print("Calculated means")



for( walkSpeed in unique(data$walk.speed.0) ) {
  for( luggageCount in unique(data$luggage.count.0)) {
    print(paste("WalkSpeed_",walkSpeed, "_luggageCount_",luggageCount, ".pdf", sep=""))
    limitedData = data %>% subset(walk.speed.0==walkSpeed & luggage.count.0==luggageCount)
    
    print(paste("Subset rows:", nrow(limitedData)));
    
    generatedPlot =
      data %>% ggplot(aes(x = boardingName, y = Iterations, color = factor(boardingName))) + 
      geom_boxplot()+
      geom_point(data=means, aes(y = meanIterations, group = factor(boardingName)), 
                 shape = 4,
                 size = 4,
                 position = position_dodge(width = 0.8)) +
      labs(title = paste("Walk Speed: ",walkSpeed, "; Luggage Count:",luggageCount, sep=""), y="Iterations") +
        theme(
          axis.text.x = element_blank(),
          axis.title.x = element_blank()
        )
      
      ggsave(paste("WalkSpeed_",walkSpeed, "_luggageCount_",luggageCount, ".pdf", sep=""), plot = generatedPlot, device = "pdf", path = "plots/boxplots",
             scale = 1, width = 20, height = 10, units = "cm",
             dpi = 300, limitsize = TRUE)
  }
}



