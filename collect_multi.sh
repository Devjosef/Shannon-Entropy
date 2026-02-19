#!/bin/bash
# Load .env
if [ -f .env ]; then
    set -a
    source .env
    set +a
else
    echo "Error: .env file missing"
    exit 1
fi

TOKEN="$FINNHUB_API_KEY"
if [ -z "$TOKEN" ]; then
    echo "Error: FINNHUB_API_KEY not set in .env"
    exit 1
fi

cd "$(dirname "$0")"
SYMBOLS=("SPY" "QQQ" "AAPL" "TSLA")
MAX_LOOPS=50

echo "Starting data collection for ${#SYMBOLS[@]} symbols... (Ctrl+C to stop)"

count=0
while [ $count -lt $MAX_LOOPS ]; do
    for sym in "${SYMBOLS[@]}"; do
        DATA=$(curl -s "https://finnhub.io/api/v1/quote?symbol=$sym&token=$TOKEN")
        
        if [[ "$DATA" == *"429"* ]]; then
            echo "Rate limit hit for $sym! waiting longer..."
            sleep 60
            break 2
        elif [[ "$DATA" == *"error"* ]]; then
            echo "API error for $sym: $DATA"
            sleep 5
        else
            if ./accumulator "$DATA"; then
                echo "$(date '+%H:%M:%S') $sym data saved"
            else
                echo "Failed to save data for $sym: $DATA"
            fi
            sleep 15
        fi
    done
    
    ((count++))
    echo "Loop $count/$MAX_LOOPS completed. Waiting before next round..."
    sleep 30
done

echo "Done! Check tests/*.csv"
