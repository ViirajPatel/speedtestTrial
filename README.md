# LCM Speedtest (Prototype - C Version)

Minimal scaffold of a speedtest daemon intended for an LCM-enabled container on prplOS.

## Features (Current Prototype)
- Autotools build (`configure && make && make install`)
- HTTP listener (default port 8080)
- Endpoint `GET /api/run` runs a simulated speed test (stub values)
- Endpoint (non-run request) returns last result (initially NO_TEST)

## Next Steps
- Integrate real iperf3 invocation
- Add scheduling, history persistence, USP integration
- Harden networking and add configuration file handling

## Build Locally
```bash
./autogen.sh   # if you add one, or run autoreconf -fi
./configure --prefix=/usr
make -j$(nproc)
```

Run:
```bash
./src/speedtestd 8080
curl http://127.0.0.1:8080/api/run
```
