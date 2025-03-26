## ----energy.sac.generation, echo=FALSE, message=FALSE, fig.height=2.75, fig.cap="Energy vs. time for the chromosome generation problem. Point size changes with chromosome size."----
library(knitr)
zig.gen.bool <- read.csv("../data/sac-gen-bool-25-Sep-19-46-01.csv")
zig.gen.bool$data.structure <- "bvector"
zig.gen.string <- read.csv("../data/sac-gen-string-25-Sep-19-44-59.csv")
zig.gen.string$data.structure <- "string"
cpp.gen.bool <- read.csv("../data/c++-bvector-gen.csv")
cpp.gen.bool$data.structure <- "bvector"
cpp.gen.string <- read.csv("../data/cpp-string-27-Sep-20-36-43.csv")
cpp.gen.string$data.structure <- "string"

gen.data <- rbind(zig.gen.bool, zig.gen.string, cpp.gen.bool, cpp.gen.string)
gen.data$language <- c(rep("zig", nrow(zig.gen.bool)), rep("zig", nrow(zig.gen.string)), rep("c++", nrow(cpp.gen.bool)), rep("c++", nrow(cpp.gen.string)))
gen.data$point.size <- log2(gen.data$size)-7

library(dplyr)
gen.data %>% group_by(language, data.structure, size ) %>% summarise(mean(PKG), sd(PKG), mean(seconds), sd(seconds)) -> gen.data.summary

library(ggplot2)

energy_vs_time <- ggplot(gen.data, aes(x=seconds, y=PKG, color=language)) +
    geom_point(aes(fill=data.structure, shape=data.structure, size=point.size), alpha=0.7, stroke=0.5) +
    theme_minimal() +
    geom_smooth(method="lm", aes(group=interaction(language, data.structure))) +
    labs(x="Time (s)", y="Energy (J)") +
    guides(size="none")

ggsave("../img/energy_vs_time.png", energy_vs_time, width=6, height=4)

gen.data$size <- as.factor(gen.data$size)


## ----energy.sac.ops, echo=FALSE, message=FALSE, fig.height=2.75, fig.cap="Boxplot of PKG energy consumption using the bit string chromosome for the two languages and different sizes",  fig.align="center"----
zig.ops.bool <- read.csv("../data/sac-2025-zig-ops-bool.csv")
zig.ops.bool$data.structure <- "bvector"
zig.ops.bool$delta.PKG <- zig.ops.bool$PKG - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(PKG)`,30))
zig.ops.bool$delta.seconds <- zig.ops.bool$seconds - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
zig.ops.string <- read.csv("../data/sac-2025-zig-ops-string.csv")
zig.ops.string$data.structure <- "string"
zig.ops.string$delta.PKG <- zig.ops.string$PKG - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PKG)`,30))
zig.ops.string$delta.seconds <- zig.ops.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
cpp.ops.bool <- read.csv("../data/cpp-bool-ops-30-Sep-09-03-07.csv")
cpp.ops.bool$data.structure <- "bvector"
cpp.ops.bool$delta.PKG <- cpp.ops.bool$PKG - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(PKG)`,30))
cpp.ops.bool$delta.seconds <- cpp.ops.bool$seconds - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="bvector" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
cpp.ops.string <- read.csv("../data/cpp-string-ops-30-Sep-09-00-11.csv")
cpp.ops.string$data.structure <- "string"
cpp.ops.string$delta.PKG <- cpp.ops.string$PKG - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PKG)`,30))
cpp.ops.string$delta.seconds <- cpp.ops.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))

ops.data <- rbind(zig.ops.bool, zig.ops.string, cpp.ops.bool, cpp.ops.string)
ops.data$language <- c(rep("zig", nrow(zig.gen.bool)), rep("zig", nrow(zig.gen.string)), rep("c++", nrow(cpp.gen.bool)), rep("c++", nrow(cpp.gen.string)))
ops.data$point.size <- log2(ops.data$size)-7

ops.data %>% group_by(language, data.structure, size) %>% summarise(mean(delta.PKG), sd(delta.PKG), mean(delta.seconds), sd(delta.seconds)) -> ops.data.summary

# ggplot(ops.data, aes(x=delta.seconds, y=delta.PKG, color=language)) + geom_point(aes(fill=data.structure,shape=data.structure,size=point.size)) + theme_minimal() + labs(x="Time (s)", y="Energy (J)") + theme(legend.position="none")

ops.data$size <- as.factor(ops.data$size)
ops_energy_vs_chromosome_size <- ggplot(ops.data[ops.data$data.structure=="string",], aes(x=size, y=PKG, color=language)) + geom_boxplot(notch=T) + geom_point(aes(fill=language,shape=language)) + theme_minimal() + labs(x="Chromosome size", y="Energy (J)")+ggtitle("Genetic operators energy consumption for bitstring chromosomes")
ggsave("../img/ops-string-boxplot.png", ops_energy_vs_chromosome_size, width=6, height=4)
string.ops.data <- ops.data[ops.data$data.structure=="string",]
differences.512 <- t.test(string.ops.data[string.ops.data$size==512 & string.ops.data$language=="c++",]$PKG, string.ops.data[string.ops.data$size==512 & string.ops.data$language=="zig",]$PKG,alternative = "less")
differences.1024 <- t.test(string.ops.data[string.ops.data$size==1024 & string.ops.data$language=="c++",]$PKG, string.ops.data[string.ops.data$size==1024 & string.ops.data$language=="zig",]$PKG,alternative = "less")
differences.2048 <- t.test(string.ops.data[string.ops.data$size==2048 & string.ops.data$language=="c++",]$PKG, string.ops.data[string.ops.data$size==2048 & string.ops.data$language=="zig",]$PKG, alternative = "greater")
string.ops.data$power <- string.ops.data$PKG/string.ops.data$seconds


## ----energy.sac.hiff, echo=FALSE, message=FALSE, fig.height=2.75, fig.cap="Delta energy from baseline vs. delta time from baseline for the HIFF fitness function for bitstring chromosomes."----
zig.hiff.string <- read.csv("../data/sac-hiff-string-1-Oct-17-33-52.csv")
zig.hiff.string$delta.PKG <- zig.hiff.string$PKG - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PKG)`,30))
zig.hiff.string$delta.seconds <- zig.hiff.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="zig" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
zig.hiff.string$language <- "zig"
cpp.hiff.string <- read.csv("../data/cpp-string-hiff-1-Oct-17-39-37.csv")
cpp.hiff.string$delta.PKG <- cpp.hiff.string$PKG - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(PKG)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(PKG)`,30))
cpp.hiff.string$delta.seconds <- cpp.hiff.string$seconds - c(rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==512,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==1024,]$`mean(seconds)`,30), rep(gen.data.summary[gen.data.summary$language=="c++" & gen.data.summary$data.structure=="string" & gen.data.summary$size==2048,]$`mean(seconds)`,30))
cpp.hiff.string$language <- "c++"

hiff.data <- rbind(zig.hiff.string, cpp.hiff.string)
hiff.data$size <- factor(hiff.data$size)
delta_energy_vs_time_hiff <- ggplot(hiff.data, aes(x=delta.seconds, y=delta.PKG, color=language, shape=size)) + geom_point() + theme_minimal() + labs(x="Delta time from baseline (s)", y="Delta energy from baseline (J)") + ggtitle("Δ energy vs. Δ time HIFF, bitstring chromosomes.") + scale_shape_manual(values=c(1,2,3)) + scale_color_manual(values=c("blue", "red")) + theme(legend.position="bottom")
ggsave("../img/delta_energy_vs_time_hiff.png", delta_energy_vs_time_hiff, width=6, height=4)

## ----sac.hiff.opsxjoule, echo=F, message=F---------------------------------------------------------
hiff.data$ops.joule <- 40000/hiff.data$delta.PKG

hiff.data %>% group_by(language, size) %>% summarise(`mean(delta.PKG)`=mean(delta.PKG), `sd(delta.PKG)`=sd(delta.PKG), `mean(delta.seconds)`=mean(delta.seconds), `sd(delta.seconds)`=sd(delta.seconds), `mean(ops.joule)`=mean(ops.joule), `sd(ops.joule)`=sd(ops.joule)) -> hiff.data.summary

library(kableExtra)

hiff.data.summary$Joules <- paste0(round(hiff.data.summary$`mean(delta.PKG)`,2), " (", round(hiff.data.summary$`sd(delta.PKG)`,2), ")")
hiff.data.summary$"Ops per Joule" <- paste0(round(hiff.data.summary$`mean(ops.joule)`,2), " (", round(hiff.data.summary$`sd(ops.joule)`,2), ")")

hiff.data.summary %>% select(language, size, Joules, "Ops per Joule") %>% arrange(size) %>% kable(digits=2, caption="\\protect\\label{tab:opsxjoule}Comparison of c++ and zig, HIFF function", "html") %>% kable_styling(full_width = F)

