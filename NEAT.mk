##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=NEAT
ConfigurationName      :=Debug
WorkspacePath          :=/home/fabien/NEAT
ProjectPath            :=/home/fabien/NEAT
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fabien
Date                   :=02/03/17
CodeLitePath           :=/home/fabien/.codelite
LinkerName             :=/usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="NEAT.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := /usr/bin/x86_64-linux-gnu-g++
CC       := /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := -std=c++11 -Wall -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Gene.cpp$(ObjectSuffix) $(IntermediateDirectory)/NodeGene.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConnectionGene.cpp$(ObjectSuffix) $(IntermediateDirectory)/Genome.cpp$(ObjectSuffix) $(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix) $(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix) $(IntermediateDirectory)/Game.cpp$(ObjectSuffix) $(IntermediateDirectory)/Species.cpp$(ObjectSuffix) \
	



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix): IDGenerator.cpp $(IntermediateDirectory)/IDGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/IDGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IDGenerator.cpp$(DependSuffix): IDGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IDGenerator.cpp$(DependSuffix) -MM IDGenerator.cpp

$(IntermediateDirectory)/IDGenerator.cpp$(PreprocessSuffix): IDGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IDGenerator.cpp$(PreprocessSuffix) IDGenerator.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/Gene.cpp$(ObjectSuffix): Gene.cpp $(IntermediateDirectory)/Gene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/Gene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Gene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Gene.cpp$(DependSuffix): Gene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Gene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Gene.cpp$(DependSuffix) -MM Gene.cpp

$(IntermediateDirectory)/Gene.cpp$(PreprocessSuffix): Gene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Gene.cpp$(PreprocessSuffix) Gene.cpp

$(IntermediateDirectory)/NodeGene.cpp$(ObjectSuffix): NodeGene.cpp $(IntermediateDirectory)/NodeGene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/NodeGene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NodeGene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NodeGene.cpp$(DependSuffix): NodeGene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NodeGene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NodeGene.cpp$(DependSuffix) -MM NodeGene.cpp

$(IntermediateDirectory)/NodeGene.cpp$(PreprocessSuffix): NodeGene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NodeGene.cpp$(PreprocessSuffix) NodeGene.cpp

$(IntermediateDirectory)/ConnectionGene.cpp$(ObjectSuffix): ConnectionGene.cpp $(IntermediateDirectory)/ConnectionGene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/ConnectionGene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConnectionGene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConnectionGene.cpp$(DependSuffix): ConnectionGene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConnectionGene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConnectionGene.cpp$(DependSuffix) -MM ConnectionGene.cpp

$(IntermediateDirectory)/ConnectionGene.cpp$(PreprocessSuffix): ConnectionGene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConnectionGene.cpp$(PreprocessSuffix) ConnectionGene.cpp

$(IntermediateDirectory)/Genome.cpp$(ObjectSuffix): Genome.cpp $(IntermediateDirectory)/Genome.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/Genome.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Genome.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Genome.cpp$(DependSuffix): Genome.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Genome.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Genome.cpp$(DependSuffix) -MM Genome.cpp

$(IntermediateDirectory)/Genome.cpp$(PreprocessSuffix): Genome.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Genome.cpp$(PreprocessSuffix) Genome.cpp

$(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix): InnovationBank.cpp $(IntermediateDirectory)/InnovationBank.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/InnovationBank.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InnovationBank.cpp$(DependSuffix): InnovationBank.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InnovationBank.cpp$(DependSuffix) -MM InnovationBank.cpp

$(IntermediateDirectory)/InnovationBank.cpp$(PreprocessSuffix): InnovationBank.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InnovationBank.cpp$(PreprocessSuffix) InnovationBank.cpp

$(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix): NEAT.cpp $(IntermediateDirectory)/NEAT.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/NEAT.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NEAT.cpp$(DependSuffix): NEAT.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NEAT.cpp$(DependSuffix) -MM NEAT.cpp

$(IntermediateDirectory)/NEAT.cpp$(PreprocessSuffix): NEAT.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NEAT.cpp$(PreprocessSuffix) NEAT.cpp

$(IntermediateDirectory)/Game.cpp$(ObjectSuffix): Game.cpp $(IntermediateDirectory)/Game.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/Game.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Game.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Game.cpp$(DependSuffix): Game.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Game.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Game.cpp$(DependSuffix) -MM Game.cpp

$(IntermediateDirectory)/Game.cpp$(PreprocessSuffix): Game.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Game.cpp$(PreprocessSuffix) Game.cpp

$(IntermediateDirectory)/Species.cpp$(ObjectSuffix): Species.cpp $(IntermediateDirectory)/Species.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/Species.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Species.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Species.cpp$(DependSuffix): Species.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Species.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Species.cpp$(DependSuffix) -MM Species.cpp

$(IntermediateDirectory)/Species.cpp$(PreprocessSuffix): Species.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Species.cpp$(PreprocessSuffix) Species.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


