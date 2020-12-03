#include <string>
#include <map>
#include "clinic.h"

namespace health {

// DOCTORS PROCESSING

void Clinic::Hire(Doctor&& new_doctor) {
  staff_manager_.Hire(std::move(new_doctor));
}

void Clinic::AddDoctor(size_t id, Doctor&& new_doctor) {
  if (doctors_.find(id) != doctors_.end()) {
    throw std::logic_error("But this Doctor is working here!");
  }
  doctors_[id] = std::move(new_doctor);
  appointment_manager_.AddNewDoctorAppointment(id, doctors_[id].rating);
}

void Clinic::FireDoctor(size_t id) {
  if (doctors_.find(id) == doctors_.end()) {
    throw std::logic_error("This doctor does not work here!");
  }
  doctors_.erase(id);
}

void Clinic::UpdateDoctorRating(size_t id, double new_rating) {
  if (new_rating < 0 || new_rating > 5) {
    throw std::logic_error(
        "Doctor's rating must belong to the interval [0, 5]");
  }
  if (doctors_.find(id) == doctors_.end()) {
    throw std::logic_error("This Doctor does not work here");
  }
  doctors_.at(id).rating = new_rating;
}

// PATIENT PROCESS

void Clinic::ScheduleAppointment(const Patient& patient) {
  appointment_manager_.ScheduleAppointment(patient);
}

void Clinic::ProcessPatients(size_t id,
             const containers::BiDirectionalList<Patient>& patients) {
  for (int i = 0; i < patients.Size(); i++) {
    int accuracy = doctors_[id].MakeDiagnosis(patients[i]->value.symptoms);
    double rating_from_i = patients[i]->value.Rate(accuracy);
    clinic_listeners_[1]->OnDailyRate(id, rating_from_i);
  }
}

// WHEN THE CLINIC IS CLOSED

void Clinic::Close() {
  for (auto& listener : clinic_listeners_) {
    listener->OnClosure(day_);
  }
  day_ = (day_ + 1 == 8) ? 1 : ++day_;
  this->ResetAppointmentManager(
      appointment_manager_.GetAppointmentLimit());
}

void Clinic::ResetAppointmentManager(size_t appointment_limit) {
  AppointmentManager new_app_manager(appointment_limit, *this);
  appointment_manager_ = new_app_manager;
}

// GETTERS

size_t Clinic::NumberOfEmployee() const {
  return doctors_.size();
}

std::vector<std::pair<size_t, double>> Clinic::GetIdsAndRatings() const {
  std::vector<std::pair<size_t, double>> result;
  result.reserve(doctors_.size());
  for (auto& pair_id_doctor : doctors_) {
    result.emplace_back(pair_id_doctor.first, pair_id_doctor.second.rating);
  }
  return result;
}

// CLINIC CONSTRUCTOR

Clinic::Clinic(size_t appointment_limit, size_t max_staff, double min_rating) :
  appointment_manager_(appointment_limit, *this),
  staff_manager_(max_staff, min_rating, *this) {
  clinic_listeners_.push_back(&appointment_manager_);
  clinic_listeners_.push_back(&staff_manager_);
}

}  // namespace health
