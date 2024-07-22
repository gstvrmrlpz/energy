## ----energy.setup, echo=F,message=F,warning=F---------------------------------
library(dplyr)
library(ggplot2)
pccito.raw <- read.csv("../data/pccito-20240715-213921.csv",sep=";")
pccito.init <- pccito.raw[pccito.raw$work=="i",]
pccito.init$seed <- NULL
pccito.init$work <- NULL

pccito.init %>% group_by(X.compiler,opt,program,length) %>% summarise(mean.energy=mean(energy),sd.energy=sd(energy),mean.time = mean(time), sd.time = sd(time)) -> pccito.summary

ggplot(pccito.init,aes(x=time,y=energy,shape=X.compiler,color=program)) + geom_point() + theme_minimal() + theme(axis.text.x = element_text(angle = 90, hjust = 1)) + labs(title="Energy consumption by compiler and optimization level",x="time",y="Energy consumption (Joules)") + scale_fill_brewer(palette="Set1")

pccito.energy.lm <- lm(energy ~  program + opt + X.compiler + length, data=pccito.init)

amd.raw <- read.csv("../data/amd-20240715-170708.csv",sep=";")
amd.init <- amd.raw[amd.raw$work=="i",]
amd.init$seed <- NULL
amd.init$work <- NULL

amd.init %>% group_by(X.compiler,opt,program,length) %>% summarise(mean.energy=mean(energy),sd.energy=sd(energy),mean.time = mean(time), sd.time = sd(time)) -> amd.summary

ggplot(amd.init,aes(x=time,y=energy,shape=X.compiler,color=program)) + geom_point() + theme_minimal() + theme(axis.text.x = element_text(angle = 90, hjust = 1)) + labs(title="Energy consumption by compiler and optimization level",x="time",y="Energy consumption (Joules)") + scale_fill_brewer(palette="Set1")

amd.energy.lm <- lm(energy ~  program + opt + X.compiler + length, data=amd.init)



