##############################################################
# Makefile to build the glcd library distribution
#
# build process overview:
#
# - Checkout a glcd library from SVN
# - create the buildinfo files
# - Remove the SVN tracking stuff from the working directory
# - update/create any needed documenation in the working directory
# - remove any uneeded/unwanted subdirectories from the working directory
# - create a zip file of the working directory.
#
# Build requires SVN, doxygen, and 7zip command line tools
#
############################################################
#
# Windoze users:
#
# you must have a commandline SVN tool:
#	http://www.open.collab.net/downloads/subversion/
#
# you must have 7zip installed.
#	http://www.7-zip.org/
# 	Make sure to manually add the install dir to your path as the
#	default install does not do this.
# 	normally it is: $SystemDrive:\Program Files\7-Zip
#
# you must have doxygen/doxywizard installed.
#	http://www.doxygen.nl/download.html#latestsrc
#	(Make sure to have version 1.6.3 or later)
#
############################################################


MYDATE := `date +%Y%m%d`
MYTIME := `date +%H%M%S`
MYDATETIME := `date`

#
#  note/warning the paths needs to be full vs relative on widows
#  as windows svn has special code to deal with the @#@$@! 
#  windows long path issue but it only kicks in on absolute paths.
#
# so on windows since TEMP is defined as a full path we use that
# vs using a local folder
#
ifndef TEMP
TEMP = .
endif

#
# LOG file name 
#
LOGFILE = $(PWD)/make.log


##################################################################
#
# Define working variables
#
##################################################################


#
# name of library
# This name will be used for the name of the distribution
# directory inside the zip file.
# it is the name of the directory that will be in the
# arduinoXXX/hardware/libraries
#
GLCDLIBNAME = glcd

#
# location of GLCD lib SVN repository
#

GLCDREPO = http://glcd-arduino.googlecode.com/svn/trunk/glcd

#
# SVN commands
#
SVNCMD = svn checkout
SVNVERSION = svnversion

#
# zip command
#
ZIPCMD = 7z a

#
# misc commands
#
RM = rm -f
RMDIR = rm -rf
CPDIR = cp -r

#
# Name of working directory for distibution tree
#
GLCDDISTDIR = $(TEMP)/$(GLCDLIBNAME)

#
# Doxygen command and config file stuff
#  note: doxygen must be started in the directory
#  where the config file is since all paths in the config file
#  are relative to that point.
#
DOXYGEN = doxygen
DOXYGENDIR = $(GLCDDISTDIR)/build/doc/doxygen
DOXYGENCFG = Doxyfile.doxcfg

#
# name of Build Information file
#
GLCDBUILDINFO = $(GLCDDISTDIR)/Buildinfo.txt
GLCDBUILDINFO_HDR = $(GLCDDISTDIR)/glcd_Buildinfo.h

#
# names of build string defines
#
GLCD_GLCDLIB_DATESTR_NAME = GLCD_GLCDLIB_DATESTR
GLCD_GLCDLIB_BUILDSTR_NAME = GLCD_GLCDLIB_BUILDSTR

#
# get the build version from SVN
# (gets expanded when used which is after svn tree is checked out)
#
GLCDBUILDVERSION := $(SVNVERSION)

#
# list of unwanted directories
# these are all relative the top level library directory
# i.e. "glcd"
#
GLCDUNWANTED = debug build

# files/directories that no matter where they are,
# remove them from the GLCD dist directory
# be careful with this.

GLCDTRASH = .svn


#
# Name of ZIP file
# has date as part of file name
#
GLCDZIPNAME = $(GLCDLIBNAME)-$(MYDATE).zip


.PHONY: all distdir distzip buildinfo docs clean checkoutsvn sanitizeGLDdir

all: distzip

distdir: $(GLCDDISTDIR)

distzip: $(GLCDZIPNAME)

buildinfo: $(GLCDBUILDINFO) $(GLCDBUILDINFO_HDR)


docs: $(GLCDDISTDIR)/doc/html

$(GLCDDISTDIR)/doc/html: $(DOXYGENDIR)
	@echo Building Doxygen Documents
	@echo ======== Building Doxygen Documents >> "$(LOGFILE)"
	@cd "$(DOXYGENDIR)" ; $(DOXYGEN) $(DOXYGENCFG) >> $(LOGFILE) 2>&1

clean:
	$(RMDIR) $(GLCDDISTDIR)
	$(RM) $(LOGFILE)
	$(RM) $(GLCDZIPNAME)


$(DOXYGENDIR) $(GLCDDISTDIR) $(GLCDDISTDIR)/.svn:
	@$(RMDIR) $(GLCDDISTDIR)
	@echo Checking out SVN working tree: $(GLCDDISTDIR)
	@echo ======== Checkout SVN tree to $(GLCDDISTDIR) >> "$(LOGFILE)"
	@echo $(SVNCMD) $(GLCDREPO) "$(GLCDDISTDIR)" >> "$(LOGFILE)"
	@$(SVNCMD) $(GLCDREPO) "$(GLCDDISTDIR)" >> "$(LOGFILE)"


$(GLCDBUILDINFO): $(GLCDDISTDIR)/.svn
	@echo Creating BuildInfo Text file
	@echo Distribution files created $(MYDATETIME) > "$(GLCDBUILDINFO)"
	@echo =====================================================================>>"$(GLCDBUILDINFO)"
	@echo BuildNumber $(GLCDBUILDVERSION) >> "$(GLCDBUILDINFO)"
	@echo =====================================================================>>"$(GLCDBUILDINFO)"

$(GLCDBUILDINFO_HDR): $(GLCDDISTDIR)/.svn
	@echo Creating BuildInfo Header file
	@echo // > "$(GLCDBUILDINFO_HDR)"
	@echo // $(GLCDLIBNAME) build information >> "$(GLCDBUILDINFO_HDR)"
	@echo // This headerfile is automatically generated >> "$(GLCDBUILDINFO_HDR)"
	@echo // >> "$(GLCDBUILDINFO_HDR)"
	@echo  >> "$(GLCDBUILDINFO_HDR)"
	@echo "#ifndef" __"`basename $(GLCDBUILDINFO_HDR) .h`"_h__ >> "$(GLCDBUILDINFO_HDR)"
	@echo "#define" __"`basename $(GLCDBUILDINFO_HDR) .h`"_h__ >> "$(GLCDBUILDINFO_HDR)"
	@echo  >> "$(GLCDBUILDINFO_HDR)"
	@echo  "#define $(GLCD_GLCDLIB_DATESTR_NAME)"\\t\"$(MYDATETIME)\" >> "$(GLCDBUILDINFO_HDR)"
	@echo  >> "$(GLCDBUILDINFO_HDR)"
	@echo  "#define $(GLCD_GLCDLIB_BUILDSTR_NAME)"\\t\"$(GLCDBUILDVERSION)\" >> "$(GLCDBUILDINFO_HDR)"
	@echo  >> "$(GLCDBUILDINFO_HDR)"
	@echo "#endif" >> "$(GLCDBUILDINFO_HDR)"
	

sanitizeGLCDdir: distdir
	@echo removing unwanted directories
	@cd "$(GLCDDISTDIR)" ; for unwanted in $(GLCDUNWANTED); do\
		rm -rf $$unwanted ;\
	done
	@echo removing unwanted trash files
	@cd "$(GLCDDISTDIR)" ; for trash in $(GLCDTRASH); do\
		find . -depth -name $$trash -exec rm -rf '{}' ';' ;\
	done


#
# WARNING: $@ target will not work (bug in make) with expansion of variables that use
# shell output and this variable contains a shell output from the date command
# so the variable was referenced directly rather than use $@
#
$(GLCDZIPNAME): distdir docs buildinfo sanitizeGLCDdir
	@echo Creating Zip file
	@echo ======== Creating Zip file from $(GLCDDISTDIR) >> "$(LOGFILE)"
	@$(RM) $(GLCDZIPNAME)
	@$(ZIPCMD) $(GLCDZIPNAME) "$(GLCDDISTDIR)" >> "$(LOGFILE)"
	$(RMDIR) $(GLCDDISTDIR)
	@echo ======== Zip file $(GLCDZIPNAME) created >> "$(LOGFILE)"
