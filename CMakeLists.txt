cmake_minimum_required(VERSION 3.10)
project(EscrowBackend)
set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)
include_directories(backend/include)
add_executable(escrow_server
    backend/main.cpp
    backend/src/models/User.cpp
    backend/src/models/EscrowAccount.cpp
    backend/src/db/DatabaseManager.cpp
    backend/src/services/AuthService.cpp
    backend/src/services/EscrowService.cpp
    backend/src/middleware/AuthMiddleware.cpp
    backend/src/controllers/AuthController.cpp
    backend/src/controllers/EscrowController.cpp
    backend/src/routes/AppRoutes.cpp
    backend/src/utils/ResponseHelper.cpp
)
target_link_libraries(escrow_server SQLite::SQLite3 pthread)
