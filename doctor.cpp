#include "doctor.h"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace health {

Doctor::Doctor(const std::vector<std::string>& his_clinical_pictures,
               double his_rating) {
  clinical_pictures = his_clinical_pictures;
  rating = his_rating;
}

Doctor::Doctor(Doctor&& new_doctor) noexcept {
  (*this) = std::move(new_doctor);
}

Doctor& Doctor::operator=(Doctor&& new_doctor) noexcept {
  this->salary = new_doctor.salary;
  new_doctor.salary = 0;
  this->rating = new_doctor.rating;
  new_doctor.rating = 0;
  this->title = std::move(new_doctor.title);
  this->name = std::move(new_doctor.name);
  this->age = new_doctor.age;
  new_doctor.age = 0;
  this->clinical_pictures = std::move(new_doctor.clinical_pictures);
  return *this;
}

int Doctor::MakeDiagnosis(const std::string& patient_illnesses) const {
  double counter_of_matches = 0;
  int number_of_illnesses = 0;
  std::stringstream s_1(patient_illnesses);
  std::string illness;
  while (getline(s_1, illness, ' ')) {
    if (find(clinical_pictures.begin(), clinical_pictures.end(),
                                         illness) != clinical_pictures.end()) {
        counter_of_matches++;
      }
      number_of_illnesses++;
  }
  if (number_of_illnesses == 0) {
    throw std::logic_error("This patient is OK!");
  }
  return floor(counter_of_matches / number_of_illnesses * 100);
}

}  // namespace health
