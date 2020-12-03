#include <string>
#include <vector>
#include "clinic.h"

#ifndef TESTING_TOOL_H_
#define TESTING_TOOL_H_

namespace health {

// Friend to classes Clinic, AppointmentManager, StaffManager.
class TestingTool {
 public:
  explicit TestingTool(Clinic& clinic) : clinic_(&clinic) {}
  std::vector<std::pair<size_t, double>> GetIdsAndRatings() const {
    return clinic_->GetIdsAndRatings();
  }

  std::vector<double> ReturnWeeklyRatings(size_t id) const {
    if (clinic_->staff_manager_.weekly_ratings_.find(id) ==
        clinic_->staff_manager_.weekly_ratings_.end()) {
      throw std::runtime_error("Weekly rating does not have such element");
    }
    return clinic_->staff_manager_.weekly_ratings_[id];
  }

  std::vector<Patient> GetDoctorAppointment(size_t id) const {
    std::vector<Patient> result;
    for (auto& appointment : clinic_->appointment_manager_.appointments_) {
      if (appointment.id_ == id) {
        result.reserve(appointment.queue_.Size());
        for (int i = 0; i < appointment.queue_.Size(); i++) {
          result.push_back(appointment.queue_[i]->value);
        }
        return result;
      }
    }
    throw std::logic_error("This doctor does not work here");
  }

 private:
  Clinic* clinic_;
};

}  // namespace health



#endif  // TESTING_TOOL_H_
