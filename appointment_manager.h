#include "bidirectional_list.h"
#include "clinic_listener.h"
#include <vector>

#ifndef APPOINTMENT_MANAGER_H_
#define APPOINTMENT_MANAGER_H_

namespace health {

class AppointmentManager : public ClinicListener {
 public:
  friend class TestingTool;
  class ClinicDelegate {
   public:
    virtual std::vector<std::pair<size_t, double>> GetIdsAndRatings() const = 0;
    virtual void ProcessPatients(size_t id,
                 const containers::BiDirectionalList<Patient>& patients) = 0;
    virtual ~ClinicDelegate() = default;
  };

  // CONSTRUCTORS
  AppointmentManager(size_t app_limit, ClinicDelegate& clinic);

  // ASSIGNMENT OPERATOR
  AppointmentManager& operator=(const AppointmentManager& app);

  // PROCESSING APPOINTMENTS
  void ScheduleAppointment(const Patient& patient);

  // Method to add appointment of the doctor hired in the middle of the day
  void AddNewDoctorAppointment(size_t id, double rating);

  // GETTER FOR TESTS
  size_t GetAppointmentLimit() const;

 private:
  // WHEN THE CLINIC IS CLOSED
  void OnClosure(size_t) override;
  void OnDailyRate(size_t, double) override;

  struct Appointment {
    Appointment(size_t id, double rating) : id_(id), rating_(rating) {}
    bool operator<(const Appointment& app_2) const;

    size_t id_{0};
    double rating_{0};
    containers::BiDirectionalList<Patient> queue_{};
  };
  size_t appointment_limit_{0};
  std::vector<Appointment> appointments_;
  ClinicDelegate* connected_clinic_{nullptr};
};

}  // namespace health

#endif  // APPOINTMENT_MANAGER_H_
