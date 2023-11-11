# Library Management System

This is a simple Library Management System implemented in C++ with MySQL database connectivity. The program allows users to log in as administrators or students, perform various operations such as adding, searching, issuing, returning, updating, and deleting books. It uses a Binary Search Tree (BST) to efficiently manage and search for books.

## Requirements

- C++ compiler
- MySQL Server
- MySQL Connector/C++ library
- Visual Studio 2022

## Connecting C/C++ to MySQL Database

### Download and Install MySQL Connector for C++

1. Visit the MySQL Community Downloads page.
2. Select the MySQL Connector C++ link.
3. Download and install MySQL Connector for C++.

### Download and Extract MySQL Connector

1. Download the MySQL Connector zip file for 64-bit systems.
2. Scroll down the list and click on 'No thanks' to avoid logging in.
3. Extract the contents of the downloaded zip file.

### Creating a New C++ Project

1. Open Visual Studio 2022.
2. Create a new project named 'mysql project one' using the 'mtc' project template.
3. Rename the 'libraries' folder within the created project.

### Building C/C++ Applications with MySQL Connector

1. Select the C++ file and name it main.cpp.
2. Paste the provided source code into main.cpp for connecting to the MySQL database.
3. Choose the release version of the MySQL Connector.

### Link MySQL Connector with C/C++ Application

1. In Visual Studio 2022, expand C/C++ settings.
2. In "Additional Include Directories," add the include folder of MySQL Connector.

### Configure MySQL Connector in Visual Studio 2022 for C/C++

1. Add the static con cpp macro to the project and add a semicolon.
2. In code generation settings, select the 'md' runtime library.
3. Navigate to linker settings, general settings, and add the additional library directories for MySQL Connector.
4. Specify the necessary folder paths within the documents, libraries, mysql, and lib64 folders.

### Add MySQL Connector Library to Project Dependencies

1. Navigate to the 'vs40' folder and select it within the project.
2. In the 'additional dependencies' tab, locate the 'mysql cppconn-static.lib' file in the 'lib64' folder, then the 'vs14' folder.

[Watch the tutorial on YouTube](https://www.youtube.com/watch?v=a_W4zt5sR1M)

## Usage

1. Run the program and choose the appropriate user type (admin or student).
2. Perform actions based on the selected user type:
   - For Admin: Add, display, search, issue, return, delete, or update books.
   - For Student: Search, issue, or return books.
3. Logout when done.

Feel free to explore and modify the code to meet your specific requirements.

## How to Run

1. Install a C++ compiler on your system.
2. Install and set up a MySQL Server.
3. Install the MySQL Connector/C++ library.
4. Create a new C++ project in Visual Studio and rename the 'libraries' folder.
5. Build C/C++ Applications using MySQL Connector in Visual Studio 2022.
6. Link MySQL Connector with C/C++ Application.
7. Configure MySQL Connector in Visual Studio 2022 for C/C++.
8. Add the MySQL Connector library to the project's dependencies.
9. Build and run the application successfully.
10. Update the database connection details (server, username, password, and database) in the code.
```
  const string server = "tcp://localhost:3306";
  const string username = "root";
  const string password = "your_password";
  const string database = "library";
```
11. Compile and run the program.

## Code Structure

The code consists of several components:

- **Database Initialization**: Initializes the MySQL database and creates necessary tables if they don't exist.
- **BST Implementation**: Defines a Binary Search Tree class to efficiently store and search for book information.
- **Database Loading into BST**: Populates the BST with book information from the MySQL database.
- **User Login and Registration**: Allows users (admin and student) to log in and register.
- **Book Operations**: Provides functionalities such as searching, issuing, returning, updating, and deleting books.
- **Menu-driven Interface**: Utilizes a user-friendly menu-driven interface for easy interaction.

## Usage

1. Run the program and choose the appropriate user type (admin or student).
2. Perform actions based on the selected user type:
   - For Admin: Add, display, search, issue, return, delete, or update books.
   - For Student: Search, issue, or return books.
3. Logout when done.

Feel free to explore and modify the code to meet your specific requirements.
