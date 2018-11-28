TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    libs \
    app \
    plugins

vari = getenv(CONFIG)

TRANSLATIONS += $${TARGET}_zh_CN.ts
TRANSLATIONS += $${TARGET}_en_US.ts
