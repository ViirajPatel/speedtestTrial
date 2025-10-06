# LCM Speedtest (Prototype - C Version)

Minimal scaffold of a speedtest daemon intended for an LCM-enabled container on prplOS.

## Features (Current Prototype)
- Autotools build (`configure && make && make install`)
- HTTP listener (default port 8080)
- Endpoint `GET /api/run` runs a simulated speed test (stub values)
- Endpoint (non-run request) returns last result (initially NO_TEST)

## Runtime Summary (Current State)
Tested locally (IPv4) with:
```
./src/speedtestd 8090
curl http://127.0.0.1:8090/api/run
```
Returned JSON including download/upload Mbps, latency, jitter, packet loss, timestamp and status.

## Dependencies (Build Host)
Install the usual autotools toolchain and a C compiler, e.g. (Debian/Ubuntu):
```
sudo apt-get update && sudo apt-get install -y build-essential autoconf automake libtool pkg-config
```
Nothing external (like iperf3) is required yet because measurements are stubbed.

## Next Steps
- Integrate real iperf3 invocation
- Add scheduling, history persistence, USP integration
- Harden networking and add configuration file handling

## Build Locally
```bash
./autogen.sh   # (wrapper around autoreconf -fi)
./configure --prefix=/usr
make -j$(nproc)
```

Run:
```bash
./src/speedtestd [port]
curl http://127.0.0.1:[port]/api/run
```

The optional second CLI argument (reserved for future) will specify an iperf3 target once real tests are integrated.

## Near-Term Roadmap
- Add real iperf3 invocation (parse JSON, fallback if not present)
- Provide `/api/latest` distinct from `/api/run` (so run triggers a fresh test, latest returns cached)
- Persist recent history (ring buffer -> JSON array) and expose `/api/history`
- Configuration file (e.g. `/etc/speedtestd.conf`) + environment variable overrides
- Graceful shutdown improvements (linger handling, timeout)
- Systemd / init script & Yocto packaging refinements (pin `SRCREV`, add `RDEPENDS` when iperf3 used)
- USP data model mapping & TR-369 parameter exposure

## Notes
Warnings are treated as errors (`-Werror`) to keep the codebase clean; add new code accordingly.
