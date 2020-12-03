#include <string>
#include <cmath>
#include "person.h"

#ifndef PATIENT_H_
#define PATIENT_H_

namespace health {

class Patient : public person::Person {
 public:
  std::string symptoms;
  double coefficient{0.2};

  Patient(const std::string& his_symptoms, double his_coefficient);
  Patient& operator=(const Patient& new_patient) = default;
  bool operator==(const Patient& rhs) const;
  double Rate(int diagnosis_accuracy) const;
};

}  // namespace health

#endif  // PATIENT_H_
