#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "WIFI-4AA4";
const char* password = "comedy4917equal";

WebServer server(80);

// 登录表单页面 HTML
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

// 处理根路径（显示登录表单）
void handleRoot() {
  server.send(200, "text/html", loginPage);
}

// 处理表单提交
void handleLogin() {
  if (server.method() == HTTP_POST) {
    String username = server.arg("username");
    String password = server.arg("password");

    if (username == "admin" && password == "1234") {
      server.send(200, "text/html", "<h2>Login success! 🎉</h2>");
    } else {
      server.send(200, "text/html", "<h2>Login failed ❌</h2><a href='/'>Try again</a>");
    }
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/login", handleLogin);

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
