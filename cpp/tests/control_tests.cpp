#include <cassert>
#include <limits>
#include "WorkshopAirWatch.h"
int main() {
  using namespace airwatch;
  Monitor monitor;
  Report normal=monitor.ingest(1,100);
  assert(normal.valid && normal.level==Level::Normal && !normal.ventilation_recommended);
  Report warning=monitor.ingest(2,1200);
  assert(warning.valid && warning.level==Level::Warning && warning.changed);
  assert(monitor.ingest(3,950).level==Level::Warning);
  assert(monitor.ingest(4,899).level==Level::Normal);
  assert(monitor.ingest(5,2100).level==Level::Critical);
  assert(monitor.ingest(6,1950).level==Level::Critical);
  assert(monitor.ingest(7,1500).level==Level::Warning);
  assert(monitor.ingest(8,700).level==Level::Normal);
  Report bad=monitor.ingest(8,50);
  assert(!bad.valid && bad.level==Level::Unknown && bad.ventilation_recommended);
  assert(!monitor.ingest(9,std::numeric_limits<double>::quiet_NaN()).valid);
  assert(!monitor.ingest(10,-1).valid);
  assert(monitor.ingest(11,100).valid);
  Monitor invalid(Settings(1000,900,100));
  assert(!invalid.ingest(1,200).valid);
}
