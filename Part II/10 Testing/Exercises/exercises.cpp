#include "gtest/gtest.h"
#include <functional>

using namespace std;

struct SpeedLimitDetected {
    unsigned short speed_mps;
};

struct SpeedUpdate {
    double velocity_mps;
};

struct CarDetected {
    double distance_m;
    double velocity_mps;
};

struct BreakCommand {
    double time_to_collision_s;
};

using SpeedUpdateCallback = function<void(const SpeedUpdate&)>;
using CarDetectedCallback = function<void(const CarDetected&)>;
using SpeedLimitCallback = std::function<void(const SpeedLimitDetected&)>;

struct IServiceBus {
    virtual ~IServiceBus() = default;
    virtual void publish(const BreakCommand&) = 0;
    virtual void subscribe(SpeedUpdateCallback) = 0;
    virtual void subscribe(CarDetectedCallback) = 0;
    virtual void subscribe(SpeedLimitCallback) = 0;
};

struct MockServiceBus : IServiceBus {
    void publish(const BreakCommand& cmd) override {
        commands_published++;
        last_command = cmd;
    };
    void subscribe(SpeedUpdateCallback callback) override {
        speed_update_callback = callback;
    };
    void subscribe(CarDetectedCallback callback) override {
        car_detected_callback = callback;
    };
    void subscribe(SpeedLimitCallback callback) override {
        speed_limit_callback = callback;
    };
    BreakCommand last_command{};
    int commands_published{};
    SpeedUpdateCallback speed_update_callback{};
    CarDetectedCallback car_detected_callback{};
    SpeedLimitCallback  speed_limit_callback{};
};

struct AutoBrake {
    AutoBrake(IServiceBus& bus) : collision_threshold_s{ 5 }, speed_mps{}, speed_limit{39} {
        bus.subscribe([this, &bus](const SpeedUpdate& update) {
            speed_mps = update.velocity_mps;
            if (speed_mps > speed_limit) {
                bus.publish(BreakCommand{0});
            }
        });
        bus.subscribe([this, &bus](const CarDetected& cd) {
            auto relative_velocity_mps = speed_mps - cd.velocity_mps;
            auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
            if(time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
                bus.publish(BreakCommand{ time_to_collision_s });
            }
        });
        bus.subscribe([this, &bus](const SpeedLimitDetected sld) {
            speed_limit = sld.speed_mps;
            if (speed_mps > speed_limit) {
                bus.publish(BreakCommand{0});
            }
        });
    }
    void set_collision_threshold_s(double x) {
        if(x < 1)
            throw runtime_error{ "Collision less than 1." };
        collision_threshold_s = x;
    }
    double get_collision_threshold_s() const {
        return collision_threshold_s;
    }
    double get_speed_mps() const {
        return speed_mps;
    }

    unsigned short get_speed_limit() const {
        return speed_limit;
    }

private:
    double collision_threshold_s;
    double speed_mps;
    unsigned short speed_limit;
};

struct AutoBrakeTest : ::testing::Test {
    MockServiceBus bus{};
    AutoBrake auto_brake { bus };
};

TEST_F(AutoBrakeTest, InitialSpeedLimitIsThirtyNine) {
    ASSERT_EQ(39, auto_brake.get_speed_limit());
}

TEST_F(AutoBrakeTest, SpeedLimitIsSaved) {
    bus.speed_limit_callback(SpeedLimitDetected{50});
    ASSERT_EQ(50, auto_brake.get_speed_limit());
    bus.speed_limit_callback(SpeedLimitDetected{70});
    ASSERT_EQ(70, auto_brake.get_speed_limit());
    bus.speed_limit_callback(SpeedLimitDetected{90});
    ASSERT_EQ(90, auto_brake.get_speed_limit());
}

TEST_F(AutoBrakeTest, NoAlertWhenSpeedIsLower) {
    bus.speed_limit_callback(SpeedLimitDetected{35});
    bus.speed_update_callback(SpeedUpdate{ 34});
    ASSERT_EQ(0, bus.commands_published);
}

TEST_F(AutoBrakeTest, AlertWhenSpeedIsHigher) {
    bus.speed_limit_callback(SpeedLimitDetected{35});
    bus.speed_update_callback(SpeedUpdate{ 40});
    ASSERT_EQ(1, bus.commands_published);
    ASSERT_DOUBLE_EQ(0, bus.last_command.time_to_collision_s);
}

TEST_F(AutoBrakeTest, AlertWhenSpeedLimitChangesToLower) {
    bus.speed_limit_callback(SpeedLimitDetected{35});
    bus.speed_update_callback(SpeedUpdate{ 30});
    bus.speed_limit_callback(SpeedLimitDetected{25});
    ASSERT_EQ(1, bus.commands_published);
    ASSERT_DOUBLE_EQ(0, bus.last_command.time_to_collision_s);
}

TEST_F(AutoBrakeTest, InitialCarSpeedIsZero) {
ASSERT_DOUBLE_EQ(0, auto_brake.get_speed_mps());
}

TEST_F(AutoBrakeTest, InitialSensitivityIsFive) {
ASSERT_DOUBLE_EQ(5, auto_brake.get_collision_threshold_s());
}

TEST_F(AutoBrakeTest, SensitivityGreaterThanOne) {
ASSERT_ANY_THROW(auto_brake.set_collision_threshold_s(0.5L));
}

TEST_F(AutoBrakeTest, SpeedIsSaved) {
    bus.speed_update_callback(SpeedUpdate{ 100L });
    ASSERT_EQ(100, auto_brake.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 50L });
    ASSERT_EQ(50, auto_brake.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 0L });
    ASSERT_DOUBLE_EQ(0, auto_brake.get_speed_mps());
}

TEST_F(AutoBrakeTest, NoAlertWhenNotImminent) {
    bus.speed_limit_callback(SpeedLimitDetected{100});
    auto_brake.set_collision_threshold_s(2L);
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 1000L, 50L });
    ASSERT_EQ(0, bus.commands_published);
}

TEST_F(AutoBrakeTest, AlertWhenImminent) {
    bus.speed_limit_callback(SpeedLimitDetected{100});
    auto_brake.set_collision_threshold_s(10L);
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 100L, 0L });
    ASSERT_EQ(1, bus.commands_published);
    ASSERT_DOUBLE_EQ(1L, bus.last_command.time_to_collision_s);
}