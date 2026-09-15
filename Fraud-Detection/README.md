Project Description
This project analyzes UIDAI Aadhaar biometric usage data to detect suspicious or abnormal usage patterns that may indicate fraud.
It provides interactive visualizations, raw data views, and automatic risk flags using Python and Streamlit.
🎯 Objective
To identify potential Aadhaar misuse by analyzing biometric authentication trends across states, districts, and time.
🧠 How it works
User uploads UIDAI biometric CSV data
The system performs:
Data cleaning
Aggregation by State, District, Date
Trend analysis
Outlier & anomaly detection
Risk levels are automatically generated
Visual dashboards display insights
🛠 Technologies Used
Python
Pandas
NumPy
Streamlit
Matplotlib
Plotly
📁 Project Structure
Copy code

uidai-fraud-detection/
│
├── app.py               # Main Streamlit application
├── requirements.txt    # Python dependencies
├── README.md            # Project documentation
├── data/                # Sample UIDAI datasets
└── src/                 # Data processing and analysis scripts
▶ How to Run Locally
Clone the repository:
Copy code

git clone https://github.com/Anjalijha1/uidai-fraud-detection
cd uidai-fraud-detection
Install dependencies:
Copy code

pip install -r requirements.txt
Run the app:
Copy code

streamlit run app.py
🌐 Live App
https://uidai-fraud-detection.streamlit.app
📊 Features
Upload UIDAI CSV data
View raw data and summaries
Interactive charts (state, district, trends)
Automatic fraud risk detection
Clean web dashboard
