#ifndef LORD_SUMI_WORKSHOP_AIR_WATCH_H
#define LORD_SUMI_WORKSHOP_AIR_WATCH_H
#include <math.h>
namespace airwatch {
enum class Level { Unknown, Normal, Warning, Critical };
struct Settings {
  double warning,critical,clear_margin;
  Settings(double w=1000.0,double c=2000.0,double m=100.0)
    :warning(w),critical(c),clear_margin(m){}
};
struct Report {
  bool valid;
  Level level;
  bool changed,ventilation_recommended;
  Report(bool v,Level l,bool c,bool vent)
    :valid(v),level(l),changed(c),ventilation_recommended(vent){}
};
inline bool valid_settings(const Settings& s) {
  return isfinite(s.warning) && isfinite(s.critical) && isfinite(s.clear_margin) &&
    s.warning>0 && s.critical>s.warning && s.clear_margin>=0 &&
    s.clear_margin<s.warning;
}
class Monitor {
 public:
  explicit Monitor(Settings settings=Settings())
    :settings_(settings),level_(Level::Normal),have_time_(false),last_timestamp_(0){}
  Report ingest(double timestamp,double ppm) {
    if (!valid_settings(settings_) || !isfinite(timestamp) ||
        (have_time_ && timestamp<=last_timestamp_) || !isfinite(ppm) || ppm<0) {
      // Unknown is advisory and must not be mistaken for a safe measurement.
      return Report(false,Level::Unknown,true,true);
    }
    Level previous=level_;
    if (ppm>=settings_.critical ||
        (previous==Level::Critical && ppm>=settings_.critical-settings_.clear_margin))
      level_=Level::Critical;
    else if (ppm>=settings_.warning) level_=Level::Warning;
    else if ((previous==Level::Warning || previous==Level::Critical) &&
             ppm>=settings_.warning-settings_.clear_margin) level_=Level::Warning;
    else level_=Level::Normal;
    have_time_=true; last_timestamp_=timestamp;
    return Report(true,level_,level_!=previous,level_!=Level::Normal);
  }
 private:
  Settings settings_;
  Level level_;
  bool have_time_;
  double last_timestamp_;
};
} // namespace airwatch
#endif
