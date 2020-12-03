#include "appointment_manager.h"
#include "staff_manager.h"
#include <map>

#ifndef CLINIC_H_
#define CLINIC_H_

namespace health {

class Clinic : public AppointmentManager::ClinicDelegate,
               public StaffManager::ClinicDelegate {
 public:
  friend class TestingTool;

  Clinic(size_t appointment_limit, size_t max_staff, double min_rating);
  void Close();

  void Hire(Doctor&& new_doctor);
  void ScheduleAppointment(const Patient& patient);

  // GETTERS
  size_t NumberOfEmployee() const override;

 private:
  // PATIENT & DOCTORS PROCESSING
  void AddDoctor(size_t id, Doctor&& new_doctor) override;
  void ProcessPatients(size_t id,
       const containers::BiDirectionalList<Patient>& patients) override;
  std::vector<std::pair<size_t, double>> GetIdsAndRatings() const override;

  // WHEN THE CLINIC IS CLOSED
  void ResetAppointmentManager(size_t appointment_limit);
  void UpdateDoctorRating(size_t id, double new_rating) override;
  void FireDoctor(size_t id) override;

  size_t day_{1};

  std::map<size_t, Doctor> doctors_;
  std::vector<ClinicListener*> clinic_listeners_{};  // Appointment, Staff.

  AppointmentManager appointment_manager_;
  StaffManager staff_manager_;
};


}  // namespace health

#endif  // CLINIC_H_
