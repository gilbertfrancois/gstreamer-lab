import os

lines = [
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-net",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-dvd-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-effects-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-editing",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-codecs-gpl-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-devtools",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-encoding",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-devtools-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-core-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-codecs-restricted",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-net-restricted-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-system-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-codecs-devel",
        "org.freedesktop.gstreamer.x86_64.base-system-1.0",
        "org.freedesktop.gstreamer.x86_64.GStreamer",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-encoding-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-codecs-gpl",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-playback-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-visualizers-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-system",
        "org.freedesktop.gstreamer.x86_64.base-crypto",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-net-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-dvd",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-libav-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-core",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-codecs",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-capture",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-capture-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-effects",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-net-restricted",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-visualizers",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-playback",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-libav",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-codecs-restricted-devel",
        "org.freedesktop.gstreamer.x86_64.base-system-1.0-devel",
        "org.freedesktop.gstreamer.x86_64.base-crypto-devel",
        "org.freedesktop.gstreamer.x86_64.gstreamer-1.0-editing-devel"
        ]

for line in lines:
    os.system(f"sudo pkgutil --forget {line}")
