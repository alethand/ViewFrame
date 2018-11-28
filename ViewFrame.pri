!isEmpty(VIEWFRAME_PRI_INCLUDED):error("VIEWFRAME.pri already included")
VIEWFRAME_PRI_INCLUDED = 1

IDE_SOURCE_TREE = $$PWD

INCLUDEPATH+= \
    $$IDE_SOURCE_TREE \
    $$IDE_SOURCE_TREE/libs \
    $$IDE_SOURCE_TREE/Plugins


defineReplace(cleanPath) {
    return($$clean_path($$1))
}

isEmpty(IDE_LIBRARY_BASENAME) {
    IDE_LIBRARY_BASENAME = lib
}

isEmpty(IDE_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_BUILD_TREE = $$cleanPath($$OUT_PWD)
    IDE_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
    IDE_LIBRARY_PATH = $$IDE_BUILD_TREE/lib
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH/plug ins
    IDE_BIN_PATH     = $$IDE_BUILD_TREE/bin
}
