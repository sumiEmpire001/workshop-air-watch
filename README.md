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
