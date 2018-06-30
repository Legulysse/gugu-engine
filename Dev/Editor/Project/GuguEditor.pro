# ***    Config  ***#

TEMPLATE = subdirs
SUBDIRS =   PugiXml \
            Sfml \
            Engine \
            EditorBase

# build must be last:
CONFIG += ordered
SUBDIRS += BuildBase
