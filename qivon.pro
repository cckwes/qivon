TEMPLATE = subdirs
SUBDIRS = core io imgproc test

io.depends = core
imgproc.depends = core
test.depends = core io imgproc
