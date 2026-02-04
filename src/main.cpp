#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
using namespace std;

// Function to display the main menu
void displayMenu() {
    cout << "\n===== VENDING MACHINE =====" << endl;
    cout << "1. Chips      - $1.25" << endl;
    cout << "2. Soda       - $1.00" << endl;
    cout << "3. Candy      - $0.75" << endl;
    cout << "4. Quit" << endl;
    cout << "===========================" << endl;
}

// Function to get the product price
double getProductPrice(int option) {
    switch(option) {
        case 1: return 1.25;  // Chips
        case 2: return 1.00;  // Soda
        case 3: return 0.75;  // Candy
        default: return -1;   // Invalid
    }
}

// Function to get the product name
string getProductName(int option) {
    switch(option) {
        case 1: return "Chips";
        case 2: return "Soda";
        case 3: return "Candy";
        default: return "Unknown";
    }
}

// Function to validate coin input
bool isValidCoin(double coin) {
    if(coin == 0.05 || coin == 0.10 || coin == 0.25 || coin == 1.00) {
        return true;
    }
    return false;
}

// Function to insert money for a purchase
double insertMoney(double productPrice) {
    double insertedMoney = 0.0;
    double coinValue = 0.0;
    
    cout << "\nProduct price: $" << fixed << setprecision(2) << productPrice << endl;
    cout << "Insert coins one at a time ($0.05, $0.10, $0.25, $1.00)" << endl;
    cout << "Enter 0 to cancel transaction." << endl;
    
    while(insertedMoney < productPrice) {
        cout << "Amount inserted so far: $" << fixed << setprecision(2) << insertedMoney;
        cout << " (Need $" << fixed << setprecision(2) << (productPrice - insertedMoney) << " more)" << endl;
        cout << "Enter coin value: $";
        cin >> coinValue;
        
        // Check if user wants to cancel
        if(coinValue == 0) {
            cout << "\nTransaction cancelled!" << endl;
            cout << "Refunding $" << fixed << setprecision(2) << insertedMoney << endl;
            return -1;  // Return -1 to indicate cancellation
        }
        
        // Validate coin
        if(!isValidCoin(coinValue)) {
            cout << "Invalid coin! Please enter $0.05, $0.10, $0.25, or $1.00" << endl;
            continue;
        }
        
        insertedMoney += coinValue;
    }
    
    return insertedMoney;
}

// Function to handle exact change curse (50% chance of jam)
bool exactChangeCurse() {
    int randomNumber = rand() % 2;  // 0 or 1
    
    if(randomNumber == 0) {
        cout << "\n*** MACHINE JAM! ***" << endl;
        cout << "The vending machine has jammed!" << endl;
        cout << "Product NOT dispensed. Your money is lost." << endl;
        return true;  // Machine jammed
    }
    else {
        cout << "\nLucky! The machine didn't jam!" << endl;
        return false;  // Machine did not jam
    }
}

// Function to return change using largest coins first
void returnChange(double changeAmount) {
    if(changeAmount <= 0.01) {
        return;  // No change needed
    }
    
    cout << "\nReturning change: $" << fixed << setprecision(2) << changeAmount << endl;
    
    // Return quarters ($0.25)
    int quarters = static_cast<int>(changeAmount / 0.25);
    if(quarters > 0) {
        cout << "Returning " << quarters << " quarter(s)" << endl;
        changeAmount -= quarters * 0.25;
    }
    
    // Return dimes ($0.10)
    int dimes = static_cast<int>(changeAmount / 0.10);
    if(dimes > 0) {
        cout << "Returning " << dimes << " dime(s)" << endl;
        changeAmount -= dimes * 0.10;
    }
    
    // Return nickels ($0.05)
    int nickels = static_cast<int>(changeAmount / 0.05);
    if(nickels > 0) {
        cout << "Returning " << nickels << " nickel(s)" << endl;
        changeAmount -= nickels * 0.05;
    }
    
    // Handle any remaining cents due to floating point precision
    if(changeAmount > 0.001) {
        cout << "Returning remaining: $" << fixed << setprecision(2) << changeAmount << endl;
    }
}

// Function to dispense product
void dispenseProduct(string productName) {
    cout << "\n*** DISPENSING " << productName << " ***" << endl;
    cout << "Your " << productName << " has been dispensed. Enjoy!" << endl;
}

int main()
{
    // Instantiate random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int menuChoice = 0;
    int sodaSelectionStreak = 0;  // Track consecutive soda selections
    bool sodaBroken = false;       // Track if soda button is broken
    
    cout << "Welcome to the Vending Machine!" << endl;
    
    // Main program loop - continues until user quits
    while(true) {
        displayMenu();
        cout << "Select an option: ";
        cin >> menuChoice;
        
        // Option 4: Quit
        if(menuChoice == 4) {
            cout << "\nThank you for using the vending machine. Goodbye!" << endl;
            break;
        }
        
        // Check for valid menu options (1-3)
        if(menuChoice < 1 || menuChoice > 4) {
            cout << "\nERROR: Invalid menu option. Please select 1, 2, 3, or 4." << endl;
            continue;
        }
        
        // Check if soda button is broken
        if(menuChoice == 2 && sodaBroken) {
            cout << "\nERROR: Soda button is broken. This product is unavailable." << endl;
            sodaSelectionStreak = 0;  // Reset streak when attempting broken button
            continue;
        }
        
        // Get product information
        double productPrice = getProductPrice(menuChoice);
        string productName = getProductName(menuChoice);
        
        cout << "\nYou selected: " << productName << " ($" << fixed << setprecision(2) << productPrice << ")" << endl;
        
        // Update soda selection streak
        if(menuChoice == 2) {
            sodaSelectionStreak++;
        }
        else {
            sodaSelectionStreak = 0;  // Reset streak if non-soda product selected
        }
        
        // Check if soda button should break (3 consecutive soda selections)
        if(sodaSelectionStreak >= 3) {
            sodaBroken = true;
            cout << "\n*** WARNING: Soda button has been used 3 times in a row! ***" << endl;
            cout << "The soda button is now broken and permanently unavailable." << endl;
            sodaSelectionStreak = 0;  // Reset for next round
            continue;  // Don't complete this purchase
        }
        
        // Insert money
        double insertedMoney = insertMoney(productPrice);
        
        // Check if transaction was cancelled
        if(insertedMoney == -1) {
            sodaSelectionStreak = 0;  // Reset on cancellation
            continue;  // Skip to next menu
        }
        
        // Check for exact change curse
        if(insertedMoney == productPrice) {
            cout << "\n*** EXACT CHANGE DETECTED ***" << endl;
            cout << "The vending machine is checking for jams..." << endl;
            
            if(exactChangeCurse()) {
                // Machine jammed - money is lost
                sodaSelectionStreak = 0;
                continue;  // Skip to next menu
            }
        }
        
        // Dispense product
        dispenseProduct(productName);
        
        // Handle change
        double changeAmount = insertedMoney - productPrice;
        if(changeAmount > 0.01) {
            returnChange(changeAmount);
        }
        else if(changeAmount < -0.01) {
            cout << "ERROR: Change calculation error!" << endl;
        }
        
        cout << "\nTransaction complete. Thank you!" << endl;
    }
    
    return 0;
}
