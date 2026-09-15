# analysis.py
import os
import pandas as pd
from PyPDF2 import PdfReader

# --- PATHS ---
base_folder = r"C:\Users\ajha4\uidai fraud detection\src"
pdf_file = os.path.join(base_folder, "yourfile.pdf")  # replace with your PDF name
csv_file = os.path.join(base_folder, "demography.csv")  # replace with CSV if available

# --- Option 1: Read PDF as text ---
if os.path.exists(pdf_file):
    reader = PdfReader(pdf_file)
    text = ""
    for page in reader.pages:
        text += page.extract_text()
    print("PDF Text Preview (first 1000 chars):")
    print(text[:1000])
else:
    print(f"PDF file not found at: {pdf_file}")

# --- Option 2: Read CSV and analyze ---
if os.path.exists(csv_file):
    df = pd.read_csv(csv_file)
    print("\nCSV Preview:")
    print(df.head())

    # Example analysis: suspicious ages > 120
    print("\nSuspicious Ages (>120):")
    print(df[df['age'] > 120])

    # Duplicate Aadhaar
    if 'aadhaar' in df.columns:
        print("\nDuplicate Aadhaar counts:")
        print(df['aadhaar'].value_counts()[df['aadhaar'].value_counts() > 1])
else:
    print(f"CSV file not found at: {csv_file}")