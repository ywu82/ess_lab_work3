#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "WIFI-4AA4";
const char* password = "comedy4917equal";

// Create a web server on port 80
WebServer server(80);

// Login form HTML page
const char* loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Login</title>
</head>
<body>
  <h1>Hi I am ESP32</h1>
  <form action="/login" method="post">
    Username: <input type="text" name="username"><br><br>
    Password: <input type="password" name="password"><br><br>
    <input type="submit" value="Login">
  </form>
</body>
</html>
)rawliteral";

// Handle root path — displays the login form
void handleRoot() {
  server.send(200, "text/html", loginPage);
}

// Handle form submission (POST request to /login)
void handleLogin() {
  if (server.method() == HTTP_POST) {
    String username = server.arg("username");
    String password = server.arg("password");

    // Simple hardcoded authentication
    if (username == "admin" && password == "1234") {
      server.send(200, "text/html", "<h2>Login success! 🎉</h2>");
    } else {
      server.send(200, "text/html", "<h2>Login failed ❌</h2><a href='/'>Try again</a>");
    }
  } else {
    // Reject methods other than POST
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Register routes
  server.on("/", handleRoot);       // Home page
  server.on("/login", handleLogin); // Login form submission

  server.begin(); // Start the web server
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient(); // Handle incoming HTTP requests
}
