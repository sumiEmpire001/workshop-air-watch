import unittest
from workshop_air_watch import AirMonitor

class AirTests(unittest.TestCase):
    def test_levels_and_hysteresis(self):
        m = AirMonitor(warning=100, critical=200, clear_margin=10)
        self.assertEqual(m.ingest(1, 120)["level"], "warning")
        self.assertEqual(m.ingest(2, 95)["level"], "warning")
        self.assertEqual(m.ingest(3, 89)["level"], "normal")
        self.assertEqual(m.ingest(4, 220)["level"], "critical")
        self.assertEqual(m.ingest(5, 195)["level"], "critical")
        self.assertEqual(m.ingest(6, 150)["level"], "warning")
    def test_validation(self):
        m = AirMonitor()
        for ppm in (-1, float("nan"), True):
            with self.assertRaises(ValueError):
                m.ingest(1, ppm)
        m.ingest(1, 100)
        with self.assertRaises(ValueError):
            m.ingest(1, 200)
        with self.assertRaises(ValueError):
            AirMonitor(100, 50)
