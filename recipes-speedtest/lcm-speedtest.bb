SUMMARY = "LCM Speedtest Daemon (prototype)"
DESCRIPTION = "Performs (simulated for now) speed tests and exposes HTTP API."
HOMEPAGE = "https://github.com/ViirajPatel/speedtestTrial"
LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=37f51597b9e89e790ea324db0d6038ee"

# Pin to a specific commit for reproducibility (replace with actual commit hash)
# TODO: Replace AUTOREV with a fixed commit once stable.
SRCREV = "${AUTOREV}"
SRC_URI = "git://github.com/ViirajPatel/speedtestTrial.git;protocol=https;branch=main"

S = "${WORKDIR}/git"

inherit autotools pkgconfig

SRC_URI += " \
    file://speedtestd.init \
    file://speedtestd.default \
"

EXTRA_OECONF = ""

do_install:append() {
    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${WORKDIR}/speedtestd.init ${D}${sysconfdir}/init.d/speedtestd
    install -d ${D}${sysconfdir}/default
    install -m 0644 ${WORKDIR}/speedtestd.default ${D}${sysconfdir}/default/speedtestd
}

FILES:${PN} += "${bindir}/speedtestd"
FILES:${PN} += "${sysconfdir}/init.d/speedtestd"
FILES:${PN} += "${sysconfdir}/default/speedtestd"
