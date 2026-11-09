"""Workshop air-quality alarm state machine (simulation, not a safety device)."""
from math import isfinite

class AirMonitor:
    def __init__(self, warning=1000.0, critical=2000.0, clear_margin=100.0):
        values = (warning, critical, clear_margin)
        if any(isinstance(v, bool) or not isinstance(v, (int, float)) or not isfinite(v) for v in values):
            raise ValueError("thresholds must be finite numbers")
        if warning <= 0 or critical <= warning or clear_margin < 0 or clear_margin >= warning:
            raise ValueError("invalid thresholds")
        self.warning = warning
        self.critical = critical
        self.clear_margin = clear_margin
        self.level = "normal"
        self.last_timestamp = None

    def ingest(self, timestamp, ppm):
        if isinstance(timestamp, bool) or not isinstance(timestamp, (int, float)) or not isfinite(timestamp):
            raise ValueError("timestamp must be finite")
        if self.last_timestamp is not None and timestamp <= self.last_timestamp:
            raise ValueError("timestamps must increase")
        if isinstance(ppm, bool) or not isinstance(ppm, (int, float)) or not isfinite(ppm) or ppm < 0:
            raise ValueError("ppm must be finite and nonnegative")
        previous = self.level
        if ppm >= self.critical:
            self.level = "critical"
        elif ppm >= self.warning:
            self.level = "warning"
        elif previous == "critical" and ppm >= self.critical - self.clear_margin:
            self.level = "critical"
        elif previous in ("warning", "critical") and ppm >= self.warning - self.clear_margin:
            self.level = "warning"
        else:
            self.level = "normal"
        self.last_timestamp = timestamp
        return {"timestamp": timestamp, "ppm": ppm, "level": self.level,
                "changed": self.level != previous, "ventilation_recommended": self.level != "normal"}
