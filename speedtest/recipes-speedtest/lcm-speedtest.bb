SUMMARY = "LCM Speedtest Daemon (prototype)"
DESCRIPTION = "Performs (simulated for now) speed tests and exposes HTTP API."
HOMEPAGE = "https://github.com/ViirajPatel/speedtestTrial"
LICENSE = "BSD-2-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=37f51597b9e89e790ea324db0d6038ee"

# Pin to a specific commit for reproducibility (replace with actual commit hash)
SRCREV = "${AUTOREV}"
SRC_URI = "git://github.com/ViirajPatel/speedtestTrial.git;protocol=https;branch=main"

S = "${WORKDIR}/git"

inherit autotools pkgconfig

EXTRA_OECONF = ""

do_install:append() {
    # (If later you add config / data dirs, install them here)
    :
}

FILES:${PN} += "${bindir}/speedtestd"
