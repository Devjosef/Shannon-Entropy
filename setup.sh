# Shannon Entropy Market Analysis Setup Script

echo "Setting up Shannon Entropy Market Analysis environment..."

# Check if virtual environment exists
if [ ! -d "venv" ]; then
    echo "Creating virtual environment..."
    python3 -m venv venv
fi

# Activate virtual environment
echo "Activating virtual environment..."
source venv/bin/activate

# Install required packages
echo "Installing required packages..."
pip install -r requirements.txt

echo "Setup complete! To activate the environment in the future, run:"
echo "source venv/bin/activate"
echo ""
echo "To run the visualization:"
echo "python visualize_entropy.py"
