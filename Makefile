.SUFFIXES: .cpp .hpp



# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf
SED	= sed
DOXYGEN = doxygen
######################################
# Project Name (generate executable with this name)
TARGET = cs296_10_exe
#Install Foler
INSTALL_FOLDER=../install
# Project Paths
PROJECT_ROOT=$(CURDIR)
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/myobjs
BINDIR = $(PROJECT_ROOT)/mybins
DOCDIR = $(PROJECT_ROOT)/doc

# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr/include/

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL
SHARED_LIB = FALSE
# Compiler and Linker flags
CPPFLAGS =-g -O3 -Wall -fno-strict-aliasing
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
LDFLAGS+=-L $(BOX2D_ROOT)/lib -L $(GLUI_ROOT)/lib
######################################

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"
######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.hpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJS_O := $(filter-out $(OBJDIR)/main.o, $(OBJS))
SRCS_O := $(filter-out $(SRCDIR)/main.cpp, $(SRCS))
.PHONY: all setup doc clean distclean exe share exelib report install dist

all: setup
	
exe: setup $(BINDIR)/$(TARGET)
	@$(PRINTF) "$(MESG_COLOR)Building executable:$(NO_COLOR) $(FILE_COLOR) %16s$(NO_COLOR)" "$(notdir $@)"
	@$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err	

exelib: setup $(BINDIR)/$(TARGET) share
	@if test $(SHARED_LIB) = TRUE; \
	then g++ -o mybins/cs296_10_exelib $(LDFLAGS)  $(OBJDIR)/main.o  $(PROJECT_ROOT)/mylibs/libCS296test.so $(LIBS); \
	else g++ -o mybins/cs296_10_exelib $(LDFLAGS)  $(OBJDIR)/main.o .//mylibs/libCS296test.a $(LIBS); \
	fi;
#g++ $(LDFLAGS) $(LD_LIBRARY_PATH) .//mylibs/libCS296test.a  -L $(LIBS) $(OBJDIR)/main.o -o mybins/cs296_10_exelib;
setup: 
	@$(ECHO) "Setting up compilation..."
	@mkdir -p myobjs
	@mkdir -p mylibs
	@mkdir -p mybins
	@if test -f $(EXTERNAL_ROOT)/include/Box2D/Box2D.h -a -f $(EXTERNAL_ROOT)/lib/libBox2D.a; \
	then $(ECHO) "INSTALLED"; \
	else cd external/src; \
	tar xvzf Box2D.tgz; \
	cd Box2D; \
	mkdir -p buildcs296; \
	cd buildcs296; \
	cmake ../; \
	make; \
	make install; \
	cd ..; \
	cd ..; \
	cd ..; \
	cd ..; \
	fi;


$(BINDIR)/$(TARGET):$(OBJS)


-include $(OBJS:.o=.d)

share: 
	@if test $(SHARED_LIB) = TRUE; \
	then $(CC) -shared -Wall $(CPPFLAGS) -o mylibs/libCS296test.so -fPIC $(SRCS_O); \
	else ar -cq mylibs/libCS296test.a $(OBJS_O); \
	fi;
$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) $(CPPFLAGS) -c $< -o $@ -MD 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else printf "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

doc:
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"

clean:
	@$(ECHO) -n "Cleaning up..."
	@$(RM) -rf $(OBJDIR) *~ $(DEPS) $(SRCDIR)/*~ my* $(DOCDIR)/html doc/*.log doc/*.aux doc/*.bbl doc/*.pdf doc/*.dvi doc/*.blg doc/*~
	@$(ECHO) "Done"

distclean: clean
	@$(RM) -rf $(BINDIR) $(EXTERNAL_ROOT)/include/* $(EXTERNAL_ROOT)/lib/* $(EXTERNAL_ROOT)/src/Box2D
report:
	@cd doc; \
	latex  cs296_report_10; \
	bibtex cs296_report_10; \
	latex cs296_report_10; \
	latex cs296_report_10; \
	latex cs296_report_10; \
	dvipdf cs296_report_10.dvi cs296_report_10.pdf;

install: setup exe doc
	@echo "installing"
	@cp -r doc $(INSTALL_FOLDER)
	@cp -r mybins/cs296_10_exe $(INSTALL_FOLDER)
	@cp -r scripts $(INSTALL_FOLDER)

dist: install distclean
	@echo $(INSTALL_FOLDER)
	@cd ..; \
	tar cvzf cs296_g10_project.tar.gz g10_project
	 
	 
	
	
