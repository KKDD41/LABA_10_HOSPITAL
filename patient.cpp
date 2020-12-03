#include "patient.h"

namespace health {

Patient::Patient(const std::string& his_symptoms,
                 double his_coefficient = 0.2) {
  symptoms = his_symptoms;
  coefficient = his_coefficient;
}

bool Patient::operator==(const Patient& rhs) const {
  return symptoms == rhs.symptoms && coefficient == rhs.coefficient;
}

double Patient::Rate(int diagnosis_accuracy) const {
  double current = coefficient * diagnosis_accuracy;
  if (current <= 5) {
    return current;
  } else {
    return current - floor(current / 5) * 5;
  }
}

}  // namespace health

