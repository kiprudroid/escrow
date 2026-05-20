# PREREQUISITES

- CROW
- SQLite
- C++
- CMake

# Installing the Prerequisites

## Windows

### CROW

1. Install a C++ compiler and build tools:
   - Install **Visual Studio 2022** or **Build Tools for Visual Studio**.
   - Make sure the **Desktop development with C++** workload is selected.

2. Install CROW:
   - CROW is a header-only library, so you can add it directly to your project.
   - Download the latest release from the CROW GitHub repository or add it as a submodule.
   - Include the CROW headers in your project and configure your include paths.

### SQLite

1. Download SQLite for Windows from the official SQLite website.
2. Extract the package and place the headers and library files in a location your project can access.
3. Add the SQLite include directory and library directory to your project settings.
4. Link against the SQLite library if your project requires it.

## Linux

### CROW

1. Install build tools:
   ```bash
   sudo apt update
   sudo apt install g++ cmake make git
   ```
2. CROW is header-only, so clone or download it and add the include path to your project.
3. If you use a package manager or system-wide install, follow the instructions for the version you are using.

### SQLite

1. Install SQLite development files:
   ```bash
   sudo apt update
   sudo apt install sqlite3 libsqlite3-dev
   ```
2. Include the SQLite headers in your project and link with `-lsqlite3` when building.
3. Verify the installation with:
   ```bash
   sqlite3 --version
   ```

# Building and Running The Backend

## Building the Backend

### Linux

1. Open a terminal and navigate to the backend directory:
   ```bash
   cd backend
   ```
2. Create a build directory and enter it:
   ```bash
   mkdir build
   cd build
   ```
3. Configure the project with CMake:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   make
   ```

### Running the Server

#### Linux

```bash
./escrow_server
```

#### Windows

```powershell
.\escrow_server.exe
```
