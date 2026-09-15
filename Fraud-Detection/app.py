import streamlit as st
import pandas as pd
import numpy as np
import plotly.express as px
from sklearn.cluster import KMeans
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split

# Page config
st.set_page_config(layout="wide")
st.title("UIDAI Biometric Dashboard")

# File uploader
file = st.file_uploader("Upload your CSV file", type=["csv"])
if file is None:
    st.info("Please upload a CSV file to see the dashboard")
    st.stop()

# Read CSV
df = pd.read_csv(file, engine="python")

# Strip any spaces from column names
df.columns = df.columns.str.strip()

# Debug: show actual columns in CSV
st.write("Columns found in CSV:", df.columns.tolist())

# Required columns for your file
required_cols = ["date", "state", "district", "bio_age_5_17", "bio_age_17_"]
missing_cols = [c for c in required_cols if c not in df.columns]
if missing_cols:
    st.error(f"Missing required columns: {missing_cols}")
    st.stop()

# Create Total_Bio
df["Total_Bio"] = df["bio_age_5_17"] + df["bio_age_17_"]

# Process dates
df["date"] = pd.to_datetime(df["date"], errors="coerce")
df = df.dropna(subset=["date"])
df["DayOfWeek"] = df["date"].dt.dayofweek
df["Month"] = df["date"].dt.month
df["IsMonthStart"] = df["date"].dt.is_month_start.astype(int)

# Dataset summary
st.subheader("Dataset Summary")
st.write(df.describe())

# Line chart: Total Bio over time
daily = df.groupby("date")["Total_Bio"].sum().reset_index()
fig1 = px.line(daily, x="date", y="Total_Bio", title="Biometric Activity Over Time")
st.plotly_chart(fig1, use_container_width=True)

# Bar chart: State-wise totals
state_data = df.groupby("state")["Total_Bio"].sum().reset_index()
fig2 = px.bar(state_data, x="state", y="Total_Bio", title="State Wise Biometric Updates")
st.plotly_chart(fig2, use_container_width=True)

# Sunburst: State → District
fig3 = px.sunburst(df, path=["state", "district"], values="Total_Bio",
                   title="State to District Biometric Activity")
st.plotly_chart(fig3, use_container_width=True)

# Heatmap: DayOfWeek vs Month
heat = df.groupby(["DayOfWeek", "Month"])["Total_Bio"].sum().reset_index()
fig4 = px.density_heatmap(heat, x="DayOfWeek", y="Month", z="Total_Bio",
                          title="Activity Heatmap")
st.plotly_chart(fig4, use_container_width=True)

# KMeans clustering: Risk clusters (Low / Medium / High)
st.subheader("District Risk Clusters")
district_data = df.groupby("district")["Total_Bio"].sum().reset_index()

# KMeans clustering
kmeans = KMeans(n_clusters=3, n_init=10, random_state=42)
district_data["RiskCluster_Num"] = kmeans.fit_predict(district_data[["Total_Bio"]])

# Map cluster numbers to Low/Medium/High based on cluster center values
cluster_centers = kmeans.cluster_centers_.flatten()
cluster_order = np.argsort(cluster_centers)  # sort cluster centers ascending
cluster_map = {cluster_order[0]: "Low Risk",
               cluster_order[1]: "Medium Risk",
               cluster_order[2]: "High Risk"}

district_data["RiskCluster"] = district_data["RiskCluster_Num"].map(cluster_map)

# Plot
fig5 = px.scatter(district_data, x="district", y="Total_Bio",
                  color="RiskCluster", title="District Risk Clusters")
st.plotly_chart(fig5, use_container_width=True)

# Show table
st.dataframe(district_data.sort_values("Total_Bio", ascending=False))

# Random Forest: Future prediction
st.subheader("Future Biometric Activity Prediction")
model_df = df.groupby("date")["Total_Bio"].sum().reset_index()
model_df["Day"] = model_df["date"].dt.day
model_df["Month"] = model_df["date"].dt.month

X = model_df[["Day", "Month"]]
y = model_df["Total_Bio"]

if len(X) > 10:
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    model = RandomForestRegressor(n_estimators=100, random_state=42)
    model.fit(X_train, y_train)

    last_month = model_df["Month"].iloc[-1]
    future = np.array([[d, last_month] for d in range(1, 11)])
    preds = model.predict(future)

    future_df = pd.DataFrame({
        "Day": range(1, 11),
        "Predicted_Total_Bio": preds
    })

    fig6 = px.line(future_df, x="Day", y="Predicted_Total_Bio",
                   title="Next 10 Days Prediction")
    st.plotly_chart(fig6, use_container_width=True)
    st.dataframe(future_df)
else:
    st.write("Not enough data for prediction")
    