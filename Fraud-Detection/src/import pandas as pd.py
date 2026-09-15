import pandas as pd

# Load datasets
bio = pd.read_csv("data/api_data_andhra_biometric-20260111T2238.csv")
demo = pd.read_csv("data/api_data_andhra_demographic-20260111T2238.csv")
enroll = pd.read_csv("data/api_data_andhra_enrollment-20260111T2238.csv")

print("Biometric data:", bio.shape)
print("Demographic data:", demo.shape)
print("Enrollment data:", enroll.shape)

print("\nBiometric Sample:")
print(bio.head())

print("\nDemographic Sample:")
print(demo.head())

print("\nEnrollment Sample:")
print(enroll.head())