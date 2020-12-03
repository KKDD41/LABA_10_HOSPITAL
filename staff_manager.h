#include "clinic_listener.h"
#include <map>

#ifndef STAFF_MANAGER_H_
#define STAFF_MANAGER_H_

namespace health {

class StaffManager : public ClinicListener {
 public:
  friend class TestingTool;
  class ClinicDelegate {
   public:
    virtual size_t NumberOfEmployee() const = 0;
    virtual void AddDoctor(size_t id, Doctor&& new_doctor) = 0;
    virtual void FireDoctor(size_t id) = 0;
    virtual void UpdateDoctorRating(size_t id, double new_rating) = 0;

    virtual ~ClinicDelegate() = default;
  };

  // CONSTRUCTORS
  StaffManager(size_t max_staff, double min_rating, ClinicDelegate& clinic);

  // DOCTORS PROCESSING
  void Hire(Doctor&& new_doctor);

 private:
  // WHEN THE CLINIC IS CLOSED
  void OnClosure(size_t day) override;
  void OnDailyRate(size_t id, double his_rating) override;

  // RATING PROCESSING
  double AverageRatingCount(size_t current_id);
  void CalculateRating();
  void EvaluateRating(size_t id, double week_average_rating);

  double min_rating_{0};
  size_t first_free_id_{0};
  size_t max_staff_{0};
  std::map<size_t, std::vector<double>> weekly_ratings_;
  ClinicDelegate* connected_clinic_{nullptr};
};

}  // namespace health

#endif  // STAFF_MANAGER_H_
