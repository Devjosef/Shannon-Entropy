import requests
import os
import csv
import math
import numpy as np
from datetime import datetime, timezone, timedelta
from dotenv import load_dotenv

load_dotenv()


def shannon_entropy(actions):
    counts = {0:0, 1:0, 2:0}
    for a in actions: counts[a] +=1
    total = len(actions)
    e = 0.0
    for c in counts.values():
        if c: e -= (c/total) * math.log2(c/total)
        return e
    
key = os.getenv("FINNHUB_API_KEY")
url = f"https://finnhub.io/api/v1/stock/candle?symbol=SPY&resolution=1&from={int((datetime.now(timezone.utc)-timedelta(days=2)).timestamp())}&to={int(datetime.now(timezone.utc).timestamp())}&token={key}"
data = requests.get(url).json()
prices = data["c"]
actions = [2 if prices[i+1]<prices[i] else 1 if prices[i+1]>prices[i] else 0 for i in range(len(prices)-1)]
windows = [actions[i:i+100] for i in range (0, len(actions)-100, 50)]

with open("tests/visual_inspection_data.csv", "w") as f:
    writer = csv.writer(f)
    writer.writerow(["Window_ID", "Entropy", "Volatility", "Market_Condition"])
    for i, w in enumerate(windows):
        entropy = shannon_entropy(w)
        volatility = np.std(np.diff(prices[i*50:(i+2)*50]))
        condition = "Bull" if entropy <1.2 else "Bear" if entropy<0.5 else "Mixed"
        writer.writerow([i, entropy, volatility, condition])