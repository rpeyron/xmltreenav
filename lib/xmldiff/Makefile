# xmldiff Makefile
#
# (c) 2004 - Rémi Peyronnet

SRC=xmldiff.cpp lx2_util.cpp lx2_diff.cpp lx2_ops.cpp errors.cpp
HDR=xmldiff.h lx2_util.h lx2_str.h lx2_diff.h lx2_ops.h errors.h

all: xmldiff

xmldiff: $(SRC) $(HDR)
	gcc -o xmldiff $(SRC) -lstdc++ -lxml2 -lxslt -lexslt -I /usr/include/libxml2
