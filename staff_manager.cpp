#include <string>
#include <map>
#include <vector>
#include "staff_manager.h"

namespace health {

// CONSTRUCTOR

StaffManager::StaffManager(size_t max_staff,
                           double min_rating,
                           ClinicDelegate& clinic) {
  max_staff_ = max_staff;
  min_rating_ = min_rating;
  connected_clinic_ = &clinic;
}

// STAFF PROCESSING

void StaffManager::Hire(Doctor&& new_doctor) {
  if (connected_clinic_->NumberOfEmployee() >= max_staff_) {
    throw std::runtime_error(
        "Impossible to hire: the number of workers is maximal now!");
  }
  if (new_doctor.rating < min_rating_) {
    throw std::logic_error(
        "Rating of the doctor is lower than the set minimum");
  }
  new_doctor.salary = new_doctor.rating * 1000;
  connected_clinic_->AddDoctor(first_free_id_, std::move(new_doctor));
  first_free_id_++;
}

// RATING PROCESSING

double StaffManager::AverageRatingCount(size_t current_id) {
  double sum_of_ratings{0};
  for (auto& curr_rating : weekly_ratings_[current_id]) {
    sum_of_ratings += curr_rating;
  }
  return (!weekly_ratings_[current_id].empty()) ? sum_of_ratings
      / weekly_ratings_[current_id].size() : 0;
}

void StaffManager::EvaluateRating(size_t id, double week_average_rating) {
  if (week_average_rating < min_rating_) {
    connected_clinic_->FireDoctor(id);
  } else {
    connected_clinic_->UpdateDoctorRating(id, week_average_rating);
  }
}

void StaffManager::OnDailyRate(size_t id, double his_rating) {
  if (weekly_ratings_.find(id) == weekly_ratings_.end()) {
    weekly_ratings_[id] = {his_rating};
  } else {
    weekly_ratings_[id].push_back(his_rating);
  }
}

// WHEN THE CLINIC IS CLOSED

void StaffManager::OnClosure(size_t day) {
  if (day != 7) {
    return;
  }
  this->CalculateRating();
}

void StaffManager::CalculateRating() {
  for (auto& pair_id_rating : weekly_ratings_) {
    EvaluateRating(pair_id_rating.first,
                   AverageRatingCount(pair_id_rating.first));
  }
  weekly_ratings_.clear();
}

}  // namespace health
