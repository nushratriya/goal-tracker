#include <iostream>
#include <string>
#include <chrono> 
#include <iomanip> 
#include <sstream> 
#include <fstream> 
#include <cctype> 

using namespace std;

struct Goal {
  string name; 
  int streak = 0; 
  string lastCompletedDate; 
};

Goal goals[100]; 
int goalCount = 0; 

string getCurrentDate() {
  auto now = chrono::system_clock::now(); 
  auto timeT = chrono::system_clock::to_time_t(now); 
  tm* localTime = localtime(&timeT); 

  stringstream dateStream; 
  dateStream << 1900 + localTime->tm_year << "-" << setw(2) << setfill('0') << 1 + localTime->tm_mon << "-" << setw(2) << setfill('0') << localTime->tm_mday; 

  return dateStream.str();
}

string toLowerCase(const string& str) {
  string lowerStr = str; 
  for (char &c : lowerStr) {
    c = ::tolower(c); 
  }
  return lowerStr; 
}

bool goalExists(const string& goalName) {
  string goalNameLower = toLowerCase(goalName); 
  for (int i = 0; i < goalCount; i++) {
    if (toLowerCase(goals[i].name) == goalNameLower) { 
      return true;
    }
  }
  return false; 
}

void loadGoals() {
  ifstream inFile("goals.txt"); 

  if (!inFile) { 
    cout << "No previous goal data found. Start working towards your goals today!" << endl; 
    return; 
  }

  string name;
  while (getline(inFile, name)) { 
    Goal newGoal; 
    newGoal.name = name; 

    inFile >> newGoal.streak; 
    inFile.ignore(); 
    getline(inFile, newGoal.lastCompletedDate);

    goals[goalCount++] = newGoal;
  }
  inFile.close(); 
  cout << "Previous goals loaded successfully." << endl; 
  cout << endl;
}

void saveGoals() {
  ofstream outFile("goals.txt");  

  for (int i = 0; i < goalCount; i++) {
    outFile << goals[i].name << endl; 
    outFile << goals[i].streak << endl; 
    outFile << goals[i].lastCompletedDate << endl; 
  }
  outFile.close(); 
}

void addGoal () {
  cout << endl;
  cout << "Enter the goal name: "; 
  string goalName;
  cin.ignore(); 
  getline(cin, goalName); 
  
  if (goalExists(goalName)) {
    cout << "Goal " << goalName << " already exists!" << endl;
    return; 
  }

  Goal newGoal; 
  newGoal.name = goalName; 
  newGoal.streak = 0; 
  newGoal.lastCompletedDate = ""; 

  goals[goalCount++] = newGoal; 
  saveGoals(); 

  cout << "" << goalName << " goal added!" << endl;
}

void markGoalCompleted() {
  cout << endl;
  cout << "Enter the goal you completed: ";
  string goalName;
  cin.ignore(); 
  getline(cin, goalName); 

  bool found = false;
  for (int i = 0; i < goalCount; i++) {
    if (toLowerCase(goals[i].name) == toLowerCase(goalName)) { 
      goals[i].streak++; 
      goals[i].lastCompletedDate = getCurrentDate(); 
      saveGoals(); 
      cout << endl;
      cout << "Good job! Streak for goal " << goals[i].name << " is now " << goals[i].streak << "." << endl;
      found = true; 
      break; 
    }
  }

  if (!found) {
    cout << "Goal " << goalName << " not found!" << endl;
  }
}

void viewAllGoals() {
  if (goalCount == 0) { 
    cout << "No goals found." << endl;
    return; 
  }
  cout << endl;
  cout << "Goal Progress: " << endl;
  for (int i = 0; i < goalCount; i++) { 
    cout << endl;
    cout << goals[i].name << ":" << endl;
    cout << "Streak: " << goals[i].streak << endl;
    cout << "Last updated on: " << (goals[i].lastCompletedDate.empty() ? "No date found" : goals[i].lastCompletedDate) << endl; 
  }
}

int main () {
  loadGoals(); 
  int choice; 

  cout << "Success is the product of daily habits, not one-time transformations. Every small action you take is a step toward building the life you want. Stay consistent, and the results will follow!" << endl;
  cout << endl;

  do {
    cout << endl;
    cout << "Goal Tracker Menu: " << endl;
    cout << "1. Add a new goal" << endl;
    cout << "2. Mark a goal as completed" << endl;
    cout << "3. View all goals" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose an option (1-4): ";
    cin >> choice;

    switch (choice) {
      case 1:
        addGoal();
        break;
      case 2:
        markGoalCompleted();
        break;
      case 3:
        viewAllGoals();
        break;
      case 4:
        saveGoals();
        cout << endl;

        cout << "Keep up the great work, I'm proud of you! :)" << endl;
        cout << "Exiting the program now, goodbye!" << endl;
        break;
      default:
        cout << "Invalid choice. Please try again." << endl;  
    }
  } while (choice != 4); 
  return 0;
}