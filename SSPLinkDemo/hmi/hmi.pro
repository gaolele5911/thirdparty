TEMPLATE = subdirs

SUBDIRS += SSPLinkHMI

!equals( QT_ARCH, arm ) {
    SUBDIRS += Test
}
