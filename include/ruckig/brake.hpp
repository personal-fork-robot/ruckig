#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include <ruckig/utils.hpp>


namespace ruckig {

//! Calculates (pre- or post-) profile to get current or final state below the limits
class BrakeProfile {
    static constexpr double eps {2.2e-14};

    static void acceleration_brake(double v0, double a0, double vMax, double vMin, double aMax, double aMin, double jMax, std::array<double, 2>& t_brake, std::array<double, 2>& j_brake);
    static void velocity_brake(double v0, double a0, double vMax, double vMin, double aMax, double aMin, double jMax, std::array<double, 2>& t_brake, std::array<double, 2>& j_brake);

public:
    //! Overall duration
    double duration {0.0};

    //! Profile information for a two-step profile
    std::array<double, 2> t, j, a, v, p;

    static void get_position_brake_trajectory(double v0, double a0, double vMax, double vMin, double aMax, double aMin, double jMax, std::array<double, 2>& t_brake, std::array<double, 2>& j_brake);
    static void get_velocity_brake_trajectory(double a0, double aMax, double aMin, double jMax, std::array<double, 2>& t_brake, std::array<double, 2>& j_brake);

    //! Finalize by integrating along kinematic state
    void finalize(double& ps, double& vs, double& as) {
        duration = t[0] + t[1];
        for (size_t i = 0; i < 2 && t[i] > 0; ++i) {
            p[i] = ps;
            v[i] = vs;
            a[i] = as;
            std::tie(ps, vs, as) = integrate(t[i], ps, vs, as, j[i]);
        }
    }
};

} // namespace ruckig
