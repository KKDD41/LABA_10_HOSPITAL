#include <string>
#include "employee.h"
#include "patient.h"
#include <set>

#ifndef DOCTOR_H_
#define DOCTOR_H_

namespace health {

class Doctor : public person::Person, public person::Employee {
 public:
  std::vector<std::string> clinical_pictures;
  double rating{0};  // from 0 to 5

  Doctor() = default;

  Doctor(const std::vector<std::string>& his_clinical_pictures, double rating);
  Doctor(Doctor&& new_doctor) noexcept;
  Doctor& operator=(Doctor&& new_doctor) noexcept;

  int MakeDiagnosis(const std::string& patient_illnesses) const;
};


}  // namespace health

#endif  // DOCTOR_H_
