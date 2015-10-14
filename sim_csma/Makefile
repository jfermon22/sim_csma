
LIB_DIR = ../lib
OBJ_DIR = ./.obj
SRC_DIR = .
DEP_DIR  = ./.deps
INCLUDE_DIR = .

# Flags passed to the preprocessor.
CPPFLAGS += -I$(INCLUDE_DIR)

# Flags passed to the C++ compiler.
CXX=g++
CXXFLAGS += -ggdb -Wall -Wextra
DEFINE_FLAGS += -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)      
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

###############
# make targets
###############
EXECS = sim_csma

all :   $(EXECS)
	
clean :
	@echo Deleting all targets
	@rm -rf $(EXECS)
	@rm -rf $(OBJ_DIR) $(DEP_DIR) $(LIB_DIR)
	


####################################################
# General rules for compiling objects and libraries
####################################################

ifeq ($(wildcard $(OBJ_DIR)/), )
$(shell mkdir -p $(OBJ_DIR))
endif

ifeq ($(wildcard $(DEP_DIR)/), )
$(shell mkdir -p $(DEP_DIR))
endif

#objects build rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling $@
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

#dependancies build rule	
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	@echo creating deps $@
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $^ | sed 's|^\(.*\.o\)|$(OBJ_DIR)/\1|' > $@

##############
# Executables
##############

#build dependecies
-include $(OBJECTS:$(OBJ_DIR)/%.o=$(DEP_DIR)/%.d)

sim_csma: $(OBJECTS)
	@echo " Linking sim_csma"
	@$(CXX)  $(CXXFLAGS) $^ -o $@
