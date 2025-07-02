## ----energy.gecco.generation, echo=FALSE, message=FALSE, fig.height=4, fig.cap="Energy vs. time for the chromosome generation problem (baseline). We use different shapes for data structures: circles for Boolean vectors, triangles for strings; languages are represented by different colors, with red representing C++ and blue zig; chromosome size is also represented by point size, with the biggest corresponding to 2048, and the smallest to 512. Lines represent a linear regression over experimental data from the same data structure."----
library(knitr)
zig.gen.bool <- read.csv("data/sea25-bool-base-11-Jan-11-54-03.csv")
zig.gen.bool$data.structure <- "bvector"
zig.gen.string <- read.csv("data/sea25-string-base-11-Jan-11-55-50.csv")
zig.gen.string$data.structure <- "string"
cpp.gen.bool <- read.csv("data/sea-bool-base-11-Jan-13-09-14.csv")
cpp.gen.bool$data.structure <- "bvector"
names(cpp.gen.bool)[names(cpp.gen.bool) == "Psys"] <- "PSys"
names(cpp.gen.bool)[names(cpp.gen.bool) == "Core"] <- "Cores"
cpp.gen.string <- read.csv("data/sea-string-base-11-Jan-13-06-25.csv")
names(cpp.gen.string)[names(cpp.gen.string) == "Psys"] <- "PSys"
names(cpp.gen.string)[names(cpp.gen.string) == "Core"] <- "Cores"
cpp.gen.string$data.structure <- "string"

gen.data <- rbind(zig.gen.bool, zig.gen.string, cpp.gen.bool, cpp.gen.string)
gen.data$language <- c(rep("zig", nrow(zig.gen.bool)), rep("zig", nrow(zig.gen.string)), rep("c++", nrow(cpp.gen.bool)), rep("c++", nrow(cpp.gen.string)))
gen.data$point.size <- log2(gen.data$size)-7

library(dplyr)
gen.data %>% group_by(language, data.structure, size ) %>% summarise(mean(PSys), sd(PSys), mean(seconds), sd(seconds)) -> gen.data.summary

library(ggplot2)
ggplot(gen.data, aes(x=seconds, y=PSys, color=language)) + geom_point(aes(fill=data.structure,shape=data.structure,size=point.size)) + theme_minimal() + geom_smooth(method="lm", aes( group=interaction(language,data.structure))) + labs(x="Time (s)", y="Energy (J)") + theme(legend.position="none")


## ----energy.gecco.generation.boxplot, echo=FALSE, message=FALSE, fig.height=4, fig.cap="Energy boxplot for the chromosome generation problem (baseline)."----
gen.data$size <- as.factor(gen.data$size)
ggplot(gen.data, aes(x=size, y=PSys, fill=language,linetype=data.structure)) + geom_boxplot() + theme_minimal() + labs(x="Data structure", y="Energy (J)")


## ----energy.gecco.histo, echo=F, message=F, fig.height=4, fig.cap="Histogram of energy consumption for the generation of Boolean vectors in C++ of length 2048",  fig.align="center"----
ggplot(cpp.gen.bool[ cpp.gen.bool$size == 1024,], aes(x=PSys)) + geom_histogram(binwidth = 2) + theme_minimal() + labs(x="Energy (J)", y="Frequency")


## ----energy.gecco.ops, echo=FALSE, message=FALSE, fig.height=4, fig.cap="Delta energy from baseline vs. delta time from baseline for genetic operators problem. Shapes and colors as in the previous Figure."----
zig.ops.bool <- read.csv("data/sea25-bool-ops-11-Jan-12-02-19.csv")
zig.ops.bool$data.structure <- "bvector"
zig.ops.bool$delta.PSys <- zig.ops.bool$PSys - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(PSys)`,30))
zig.ops.bool$delta.seconds <- zig.ops.bool$seconds - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
zig.ops.string <- read.csv("data/sea25-string-ops-11-Jan-12-00-03.csv")
zig.ops.string$data.structure <- "string"
zig.ops.string$delta.PSys <- zig.ops.string$PSys - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PSys)`,30))
zig.ops.string$delta.seconds <- zig.ops.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
cpp.ops.bool <- read.csv("data/sea-bool-ops-11-Jan-13-16-29.csv")
cpp.ops.bool$data.structure <- "bvector"
names(cpp.ops.bool)[names(cpp.ops.bool) == "Psys"] <- "PSys"
names(cpp.ops.bool)[names(cpp.ops.bool) == "Core"] <- "Cores"
cpp.ops.bool$delta.PSys <- cpp.ops.bool$PSys - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(PSys)`,30))
cpp.ops.bool$delta.seconds <- cpp.ops.bool$seconds - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
cpp.ops.string <- read.csv("data/sea-string-ops-11-Jan-13-24-20.csv")
cpp.ops.string$data.structure <- "string"
names(cpp.ops.string)[names(cpp.ops.string) == "Psys"] <- "PSys"
names(cpp.ops.string)[names(cpp.ops.string) == "Core"] <- "Cores"
cpp.ops.string$delta.PSys <- cpp.ops.string$PSys - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PSys)`,30))
cpp.ops.string$delta.seconds <- cpp.ops.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))

ops.data <- rbind(zig.ops.bool, zig.ops.string, cpp.ops.bool, cpp.ops.string)
ops.data$language <- c(rep("zig", nrow(zig.gen.bool)), rep("zig", nrow(zig.gen.string)), rep("c++", nrow(cpp.gen.bool)), rep("c++", nrow(cpp.gen.string)))
ops.data$point.size <- log2(ops.data$size)-7

ops.data %>% group_by(language, data.structure, size) %>% summarise(mean(delta.PSys), sd(delta.PSys), mean(delta.seconds), sd(delta.seconds)) -> ops.data.summary

ggplot(ops.data, aes(x=delta.seconds, y=delta.PSys, color=language)) + geom_point(aes(fill=data.structure,shape=data.structure,size=point.size)) + theme_minimal() + labs(x="Time (s)", y="Energy (J)") + theme(legend.position="none")


## ----energy.gecco.string, echo=F, message=F, fig.height=4, fig.cap="Boxplot of PSys energy consumption using the bit string chromosome for the two languages and different sizes",  fig.align="center"----
ops.data$size <- as.factor(ops.data$size)
ggplot(ops.data[ops.data$data.structure=="string",], aes(x=size, y=PSys, color=language)) + geom_boxplot(notch=T) + geom_point(aes(fill=language,shape=language)) + theme_minimal() + labs(x="Chromosome size", y="Energy (J)")
string.ops.data <- ops.data[ops.data$data.structure=="string",]
differences.512 <- t.test(string.ops.data[string.ops.data$size==512 & string.ops.data$language=="c++",]$PSys, string.ops.data[string.ops.data$size==512 & string.ops.data$language=="zig",]$PSys,alternative = "less")
differences.1024 <- t.test(string.ops.data[string.ops.data$size==1024 & string.ops.data$language=="c++",]$PSys, string.ops.data[string.ops.data$size==1024 & string.ops.data$language=="zig",]$PSys,alternative = "less")
differences.2048 <- t.test(string.ops.data[string.ops.data$size==2048 & string.ops.data$language=="c++",]$PSys, string.ops.data[string.ops.data$size==2048 & string.ops.data$language=="zig",]$PSys, alternative = "greater")


## ----energy.gecco.string.power, echo=F, message=F, fig.height=4, fig.cap="Boxplot of PSys power (energy over time) consumption using the bit string chromosome for the two languages and different sizes.",  fig.align="center"----
string.ops.data$power <- string.ops.data$PSys/string.ops.data$seconds
ggplot(string.ops.data, aes(x=size, y=power, color=language)) + geom_boxplot(notch=T) + geom_point(aes(fill=language,shape=language)) + theme_minimal() + labs(x="Chromosome size", y="Power (w)")


## ----energy.gecco.hiff, echo=FALSE, message=FALSE, fig.height=4, fig.cap="Delta energy from baseline vs. delta time from baseline for the HIFF fitness function for bitstring chromosomes in C++ and zig."----
zig.hiff.string <- read.csv("data/sea25-hiff-11-Jan-12-03-44.csv")
zig.hiff.string$delta.PSys <- zig.hiff.string$PSys - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PSys)`,30))
zig.hiff.string$delta.seconds <- zig.hiff.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
zig.hiff.string$language <- "zig"
cpp.hiff.string <- read.csv("data/sea-string-hiff-11-Jan-13-39-12.csv")
names(cpp.hiff.string)[names(cpp.hiff.string) == "Psys"] <- "PSys"
names(cpp.hiff.string)[names(cpp.hiff.string) == "Core"] <- "Cores"
cpp.hiff.string$delta.PSys <- cpp.hiff.string$PSys - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PSys)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PSys)`,30))
cpp.hiff.string$delta.seconds <- cpp.hiff.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
cpp.hiff.string$language <- "c++"

hiff.data <- rbind(zig.hiff.string, cpp.hiff.string)
hiff.data$size <- factor(hiff.data$size)
ggplot(hiff.data, aes(x=delta.seconds, y=delta.PSys, color=language, shape=size)) + geom_point() + theme_minimal() + labs(x="Delta time from baseline (s)", y="Delta energy from baseline (J)")


## ----energy.gecco.hiff.opsxjoule, echo=F, message=F----------------------------------------------------------------------------------------------
hiff.data$ops.joule <- 40000/hiff.data$delta.PSys

hiff.data %>% group_by(language, size) %>% summarise(`mean(delta.PSys)`=mean(delta.PSys), `sd(delta.PSys)`=sd(delta.PSys), `mean(delta.seconds)`=mean(delta.seconds), `sd(delta.seconds)`=sd(delta.seconds), `mean(ops.joule)`=mean(ops.joule), `sd(ops.joule)`=sd(ops.joule)) -> hiff.data.summary

library(kableExtra)

hiff.data.summary$Joules <- paste0(round(hiff.data.summary$`mean(delta.PSys)`,2), " (", round(hiff.data.summary$`sd(delta.PSys)`,2), ")")
hiff.data.summary$"Ops per Joule" <- paste0(round(hiff.data.summary$`mean(ops.joule)`,2), " (", round(hiff.data.summary$`sd(ops.joule)`,2), ")")

hiff.data.summary %>% select(language, size, Joules, "Ops per Joule") %>% arrange(size) %>% kable(digits=2, caption="\\protect\\label{tab:opsxjoule}Comparison of c++ and zig, HIFF function") %>% kable_styling(full_width = F)

