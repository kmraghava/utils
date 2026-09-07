
# Compiler
CC := gcc
AR := ar

# Package name
pkg_name := kmrUtils

# Source code directory
src_dir := src

# Build directory
build_dir := build

# Library name
lib_name := $(build_dir)/lib$(pkg_name)

# Install directory
install_dir := /usr/local

# Compiler flags
CFLAGS := -Wall -Wextra -g -fPIC

# Defines
DEFINES :=

# Includes
includes := -Iinclude

# Source files
sources := $(src_dir)/app_timer.c \
		   $(src_dir)/clist.c \
		   $(src_dir)/ctree.c \
		   $(src_dir)/json_field.c \
		   $(src_dir)/logger.c \
		   $(src_dir)/mac_addr.c \
		   $(src_dir)/msg_queue.c \
		   $(src_dir)/tree.c \
		   $(src_dir)/str.c \
		   $(src_dir)/strbuf.c \
		   \

# Object files
objects := $(patsubst %.c, $(build_dir)/%.o, $(sources))

# make all target
all: $(lib_name).so $(lib_name).a

$(lib_name).so: $(objects)
	$(CC) -shared -o $@ $^ -ljansson

$(lib_name).a: $(objects)
	$(AR) rcs $@ $^

$(build_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEFINES) $(includes) -o $@ -c $<

#make install target
install: all
	mkdir -p $(install_dir)/lib
	mkdir -p $(install_dir)/include/$(pkg_name)

	cp $(lib_name).so $(install_dir)/lib/
	cp $(lib_name).a  $(install_dir)/lib/

	cp  include/* $(install_dir)/include/$(pkg_name)/




######################################################################
#  TESTING
######################################################################

# Libraries to compile with
TLDLIBS := -ljansson -lkmrUtils

# sources
test_src_dir := test

# Build directory for unit tests
test_build_dir := $(build_dir)/test

# unit test sources
test_sources := $(test_src_dir)/tclist.c \
                $(test_src_dir)/tctree.c \
                $(test_src_dir)/tjson_field.c \
                $(test_src_dir)/tmacro_args.c \
                $(test_src_dir)/tlist.c \
                $(test_src_dir)/tmac_addr.c \
                $(test_src_dir)/ttree.c \
                $(test_src_dir)/tstr.c \
                $(test_src_dir)/tstrbuf.c \
                $(test_src_dir)/unit_test.c \
                \

#unit test object files
test_objects := $(patsubst %.c, $(test_build_dir)/%.o, $(test_sources))

#Test target
test: $(test_build_dir)/unit_test

$(test_build_dir)/unit_test: $(test_objects)
	$(CC) -g -o $@ $^ $(TLDLIBS)

$(test_build_dir)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -g -o $@ -c $<

clean:
	rm -rf $(build_dir)

.PHONY: all clean

