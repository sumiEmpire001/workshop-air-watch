# workshop-air-watch

An in-memory workshop air-quality state machine that validates simulated PPM readings, enforces monotonic timestamps, and applies hysteresis to warning/critical alarms. Outputs a ventilation recommendation, not hardware commands. This is not a calibrated air-quality, fire, occupational safety, or life-safety device.

## Run tests

Requires Python 3.10+ with no external dependencies.

```bash
python -m unittest discover -s tests -v
```

## Example

```python
from workshop_air_watch import AirMonitor
monitor = AirMonitor()
print(monitor.ingest(1, 1200))
```

GitHub Actions runs the test suite on Python 3.10, 3.12, and 3.13. Real hardware integration and deployment remain future work.

## C++ Arduino / ESP32 implementation

The Python alarm engine is preserved and its critical-alarm hysteresis ordering
has been fixed. The C++11 equivalent lives in
`arduino/WorkshopAirWatch/src/WorkshopAirWatch.h` with unit tests. Levels are
Normal, Warning, Critical or Unknown on invalid input. An invalid timestamp or
sample produces an advisory Unknown result recommending investigation.

An Arduino `IndicatorDemo` maps a **potentiometer** to synthetic PPM and lights
an LED to indicate advisory ventilation status. It is *not* an air pollution
monitor, gas leak detector, smoke detector, or safety alarm. The Uno demo uses
A0/D8; classic ESP32 uses GPIO34/18. Install `arduino/WorkshopAirWatch` as an
Arduino library and open the packaged example.

### Build and test C++
```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

GitHub Actions builds the C++ core under GCC and Clang. Firmware compilation
on physical boards, calibrated sensor integration, alarm latency, wiring,
long-running timer rollover, and occupational safety compliance remain untested.
**Never rely on this code for human safety or hazardous air alerts.**
