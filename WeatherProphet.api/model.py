import torch.nn as nn
import torch.nn.functional as F

class WeatherProphet(nn.Module):
    def __init__(self):
        super(WeatherProphet, self).__init__()
        
        self.fc1 = nn.Linear(5, 64)
        self.fc2 = nn.Linear(64, 64)
        self.fc3 = nn.Linear(64, 10)
        
    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        output = self.fc3(x)
        return output