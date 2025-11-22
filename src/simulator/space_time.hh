
#pragma once

class SpaceTime {
public:
  SpaceTime() = default;
  void setup_time(double rdshft, int nstp);
  double get_time_current() const;
  double get_time_start() const;
  double get_time_end() const;
  double get_dt() const;
  double get_time_scale() const;
  double get_redshift_current() const;
  void update_time(double dt);
  void update_dt(double new_dt);
private:
  double redshft_z{0.0f};
  int nsteps{0};
  double time{0.0};
  double t0{0.0};
  double dt{0.0f};
};
