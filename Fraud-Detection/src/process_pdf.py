import pandas as pd
import glob

# Get all CSV files from the data folder
csv_files = glob.glob("../data/*.csv")

if not csv_files:
    print("❌ No CSV files found in data folder")
    exit()

print("Found CSV files:")
for f in csv_files:
    print(f)

dfs = [pd.read_csv(f) for f in csv_files]
combined = pd.concat(dfs, ignore_index=True)

combined.to_csv("../data/uidai_combined.csv", index=False)
print("✅ Combined dataset saved as ../data/uidai_combined.csv")