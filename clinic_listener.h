#ifndef CLINIC_LISTENER_H_
#define CLINIC_LISTENER_H_

#include <string>
#include "doctor.h"

class ClinicListener {
 public:
  virtual void OnClosure(size_t day) = 0;
  virtual void OnDailyRate(size_t id, double rating) = 0;
  virtual ~ClinicListener() = default;
};

#endif  // CLINIC_LISTENER_H_
