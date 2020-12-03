#include <gtest/gtest.h>
#include "testing_tool.h"

using health::Clinic;
using health::TestingTool;
using health::Doctor;
using health::Patient;

TEST(Test_0, DoctorProcessing) {
  Doctor doctor_1({"1", "2", "3", "4"}, 2);
  Doctor doctor_2({"5", "6"}, 1);
  Doctor doctor_3({"4", "5", "6", "7"}, 3);

  Patient patient_1("1 2", 0.02);
  Patient patient_2("3 4 5 6", 0.01);
  Patient patient_3("0 6", 0.1);
  Patient patient_4("228", 0.8);

  Clinic hospital(1, 2, 1);

  TestingTool test(hospital);
  std::vector<std::pair<size_t, double>> expected_list;

  // DAY 1
  EXPECT_ANY_THROW(hospital.ScheduleAppointment(patient_2));

  hospital.Hire(std::move(doctor_2));
  expected_list.emplace_back(0, 1);
  EXPECT_TRUE(test.GetIdsAndRatings() == expected_list);

  hospital.Close();

  // WEEK 1
  // DAY 2
  hospital.Hire(std::move(doctor_1));
  expected_list.emplace_back(1, 2);
  EXPECT_TRUE(test.GetIdsAndRatings() == expected_list);
  EXPECT_ANY_THROW(hospital.Hire(std::move(doctor_3)));

  hospital.ScheduleAppointment(patient_2);
  hospital.ScheduleAppointment(patient_1);
  EXPECT_ANY_THROW(hospital.ScheduleAppointment(patient_3));
  hospital.Close();

  for (int i = 3; i < 8; i++) {
    hospital.ScheduleAppointment(patient_2);
    hospital.ScheduleAppointment(patient_1);
    EXPECT_ANY_THROW(hospital.ScheduleAppointment(patient_3));
    EXPECT_TRUE(test.GetDoctorAppointment(1) ==
                        std::vector<Patient>{patient_2});
    EXPECT_TRUE(test.GetDoctorAppointment(0) ==
                        std::vector<Patient>{patient_1});

    EXPECT_EQ(test.ReturnWeeklyRatings(1)[i - 3],
                         50 * patient_2.coefficient);
    EXPECT_EQ(test.ReturnWeeklyRatings(0)[i - 3],
                         0 * patient_1.coefficient);

    EXPECT_EQ(test.ReturnWeeklyRatings(1).size(), i - 2);
    EXPECT_EQ(test.ReturnWeeklyRatings(0).size(), i - 2);
    hospital.Close();
    if (i != 7) {
      EXPECT_TRUE(test.GetDoctorAppointment(0).empty() &&
          test.GetDoctorAppointment(1).empty());
    }
  }

  // after this week rating of the doctor_1 equals to 50 * 0.01 == 0.5, and
  // rating of the doctor_2 is zero. So both doctor_1 and doctor_2 were fired!
  EXPECT_TRUE(test.GetIdsAndRatings().empty());
}

TEST(Test_1, WhenOneDoctorHasNoWork) {
  Doctor doctor_1({"1", "2", "3", "4"}, 2);
  Doctor doctor_2({"5", "6"}, 1);
  Doctor doctor_3({"4", "5", "6", "7"}, 3);
  Doctor doctor_4({"4", "5", "6", "7"}, 3);

  Patient patient_1("1 2", 0.02);
  Patient patient_2("3 4 5 6", 0.01);
  Patient patient_3("0 6", 0.1);
  Patient patient_4("228", 0.8);

  Clinic hospital(2, 3, 0.4);
  TestingTool test(hospital);

  // DAY 1
  hospital.Hire(std::move(doctor_2));
  hospital.Hire(std::move(doctor_1));
  hospital.Hire(std::move(doctor_3));
  EXPECT_EQ(test.GetIdsAndRatings().size(), 3);
  hospital.Close();

  // WEEK 1
  for (int i = 2; i < 8; i++) {
    hospital.ScheduleAppointment(patient_1);
    hospital.ScheduleAppointment(patient_2);
    EXPECT_TRUE(test.GetDoctorAppointment(2) ==
                        std::vector<Patient>{patient_1});
    EXPECT_TRUE(test.GetDoctorAppointment(1) ==
                        std::vector<Patient>{patient_2});
    EXPECT_TRUE(test.GetDoctorAppointment(0).empty());
    if (i >= 3) {
      EXPECT_TRUE(test.ReturnWeeklyRatings(2)[i - 3] ==
                          0 * patient_1.coefficient);
      EXPECT_TRUE(test.ReturnWeeklyRatings(1)[i - 3] ==
                          50 * patient_2.coefficient);
    }
    hospital.Close();
  }

  // Doctor_3 was fired, because he has rating zero after this week.
  // But doctor_2 is still with us (he does not have patients this week,
  // and we suppose that his rating does not change)
  std::vector<std::pair<size_t, double>> expected_list{
                                       std::pair<size_t, double>(0, 1),
                                       std::pair<size_t, double>(1, 0.5)};
  EXPECT_TRUE(test.GetIdsAndRatings() == expected_list);

  // We clean map weekly_ratings_ after each week and make sure, that if we
  // fired doctor_3, he won't be in the weekly_ratings_ next week.
  EXPECT_ANY_THROW(test.ReturnWeeklyRatings(2));

  hospital.Hire(std::move(doctor_4));
  hospital.Close();

  // WEEK 2
  std::vector<Patient> exp_appointment{patient_1, patient_1};
  for (int i = 2; i < 8; i++) {
    if (i % 2) {
      hospital.ScheduleAppointment(patient_1);
      hospital.ScheduleAppointment(patient_2);
      hospital.ScheduleAppointment(patient_3);
      hospital.ScheduleAppointment(patient_1);
      EXPECT_TRUE(test.GetDoctorAppointment(3) == exp_appointment);
      EXPECT_TRUE(test.GetDoctorAppointment(0) ==
                          std::vector<Patient>{patient_2});
      EXPECT_TRUE(test.GetDoctorAppointment(1) ==
                          std::vector<Patient>{patient_3});
    } else {
      hospital.ScheduleAppointment(patient_4);
      hospital.ScheduleAppointment(patient_3);
      hospital.ScheduleAppointment(patient_2);
      EXPECT_TRUE(test.GetDoctorAppointment(3) ==
                          std::vector<Patient>{patient_4});
      EXPECT_TRUE(test.GetDoctorAppointment(0) ==
                          std::vector<Patient>{patient_3});
      EXPECT_TRUE(test.GetDoctorAppointment(1) ==
                          std::vector<Patient>{patient_2});
    }
    hospital.Close();
  }

  // Doctor_4 and doctor_1 must be fired.
  expected_list.clear();
  expected_list.emplace_back(0, 2.75);
  EXPECT_TRUE(test.GetIdsAndRatings() == expected_list);
}

