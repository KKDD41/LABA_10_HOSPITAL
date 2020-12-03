#include "appointment_manager.h"
#include <algorithm>

namespace health {

bool AppointmentManager::Appointment::operator<(
    const AppointmentManager::Appointment& app_2) const {
  if (queue_.Size() != app_2.queue_.Size()) {
    return queue_.Size() < app_2.queue_.Size();
  } else {
    return rating_ > app_2.rating_;
  }
}

// CONSTRUCTOR

AppointmentManager::AppointmentManager(size_t app_limit,
                                       ClinicDelegate& clinic) {
  appointment_limit_ = app_limit;
  connected_clinic_ = &clinic;
  for (auto& pair_id_rating : connected_clinic_->GetIdsAndRatings()) {
    appointments_.emplace_back(pair_id_rating.first, pair_id_rating.second);
  }
  sort(appointments_.begin(), appointments_.end());
}

AppointmentManager& AppointmentManager::operator=(
    const AppointmentManager& app) {
  if (this == &app) {
    return *this;
  }
  connected_clinic_ = app.connected_clinic_;
  appointments_ = app.appointments_;
  appointment_limit_ = app.appointment_limit_;
  return *this;
}

// GETTER

size_t AppointmentManager::GetAppointmentLimit() const {
  return appointment_limit_;
}

// PATIENT PROCESSING

void AppointmentManager::ScheduleAppointment(const Patient& patient) {
  if (appointments_.empty()) {
    throw std::logic_error("There are no doctors in the hospital!");
  }
  sort(appointments_.begin(), appointments_.end());
  if (appointments_[0].queue_.Size() < appointment_limit_) {
    appointments_[0].queue_.PushBack(patient);
    return;
  }
  throw std::runtime_error("Unfortunately, the hospital is full!");
}

void AppointmentManager::AddNewDoctorAppointment(size_t id, double rating) {
  appointments_.emplace_back(id, rating);
}

// WHEN THE CLINIC IS CLOSED

void AppointmentManager::OnClosure(size_t) {
  for (auto& appointment : appointments_) {
    connected_clinic_->ProcessPatients(appointment.id_, appointment.queue_);
  }
}

void AppointmentManager::OnDailyRate(size_t, double) {}

}  // namespace health
