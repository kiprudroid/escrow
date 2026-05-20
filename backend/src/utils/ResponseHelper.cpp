#include "crow.h"

crow::response jsonMessage(int code, const std::string& message) {
    crow::json::wvalue result;
    result["message"] = message;
    crow::response res(code);
    res.set_header("Content-Type", "application/json");
    res.write(result.dump());
    return res;
}

crow::response jsonData(int code, crow::json::wvalue data) {
    crow::json::wvalue result;
    result["data"] = std::move(data);
    crow::response res(code);
    res.set_header("Content-Type", "application/json");
    res.write(result.dump());
    return res;
}
