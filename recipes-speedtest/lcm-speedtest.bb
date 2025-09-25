SUMMARY = "LCM Speedtest Daemon (prototype)"
DESCRIPTION = "Performs (simulated for now) speed tests and exposes HTTP API."
LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=unknown"

SRC_URI = "file://."
S = "${WORKDIR}"

inherit autotools

EXTRA_OECONF = ""

do_configure:prepend () {
    cp -r ${THISDIR}/../ ${S}/source || true
}

do_compile () {
    oe_runmake -C source
}

do_install () {
    install -d ${D}${bindir}
    install -m 0755 source/src/speedtestd ${D}${bindir}/speedtestd
}

FILES:${PN} += "${bindir}/speedtestd"
