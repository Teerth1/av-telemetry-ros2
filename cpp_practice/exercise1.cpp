/**
 * ============================================
 * C++ BOOTCAMP - Exercise 1: The Basics
 * ============================================
 * 
 * YOUR MISSION: Complete each TODO section.
 * Don't look at lane_detector_node.cpp for answers!
 * 
 * Java comparison provided in comments.
 */

#include <iostream>   // Like: import java.io.*;
#include <string>     // Like: import java.lang.String; (but C++ strings are different!)

// ============================================
// CHALLENGE 1: Create a class called "Vehicle"
// ============================================
// 
   
// In Java you'd write:
//     public class Vehicle {
//         private String brand;
//         private int speed;
//     }
//
// YOUR TASK: Write the C++ equivalent below.
// Remember:
//   - C++ uses "class ClassName { };" (note the semicolon!)
//   - Access modifiers work on BLOCKS (private: ... public: ...)
//   - Use std::string for strings
//   - Member variables often end with underscore: speed_

// TODO: Write your Vehicle class here

void Log(const char message) {
    std::cout << message << std::endl;
}


int Multiply(int a , int b) {

    return a * b;
}

int main() {
    std::cout << Multiply(2,3) << std::endl;
    return 0;

 
    std::cout << "Hello World" << std::endl;
}


// ============================================
// CHALLENGE 2: Add a Constructor
// ============================================
//
// In Java you'd write:
//     public Vehicle(String brand, int speed) {
//         this.brand = brand;
//         this.speed = speed;
//     }
//
// YOUR TASK: Add a constructor to your Vehicle class above.
// Remember:
//   - C++ uses initializer lists:  ClassName() : member1_(val1), member2_(val2) { }
//   - Or you can do it Java-style in the body with this->member = value;



// ============================================
// CHALLENGE 3: Add a method called "accelerate"
// ============================================
//
// In Java you'd write:
//     public void accelerate(int amount) {
//         this.speed += amount;
//     }
//
// YOUR TASK: Add this method to your Vehicle class.
// Remember:
//   - Return type comes BEFORE the function name
//   - Parameters work the same way



// ============================================
// CHALLENGE 4: Add a method called "getInfo"
// ============================================
//
// In Java you'd write:
//     public String getInfo() {
//         return brand + " going " + speed + " mph";
//     }
//
// YOUR TASK: Add this method to your Vehicle class.
// Remember:
//   - C++ string concatenation uses + but numbers need std::to_string()
//   - Example: "Hello " + std::to_string(42)



// ============================================
// CHALLENGE 5: Write the main function
// ============================================
//
// In Java you'd write:
//     public static void main(String[] args) {
//         Vehicle car = new Vehicle("BMW", 60);
//         car.accelerate(20);
//         System.out.println(car.getInfo());
//     }
//
// YOUR TASK: Write the C++ equivalent below.
// Remember:
//   - int main() { ... return 0; }
//   - Create object on stack: Vehicle car("BMW", 60);  (no "new" keyword!)
//   - Print with: std::cout << "text" << std::endl;

int main()
{
    // TODO: Create a Vehicle object (on the stack, not heap!)
    
    // TODO: Call accelerate(20) on it
    
    // TODO: Print the result of getInfo()
    
    return 0;
}
