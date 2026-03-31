#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(4, 5);
const byte address[6] = "00001";

ESP8266WebServer server(80);

String chatLog = "";
String key = "secret123";

int msgId = 0;
int successCount = 0, totalCount = 0;
int signalLevel = 1;

bool isTyping = false;
unsigned long lastTypingTime = 0;

// WiFi
const char* ssid = "NodeB_Chat";
const char* password = "12345678";

// ---------- ENCRYPT ----------
String encryptDecrypt(String input) {
  String output = input;
  for (int i = 0; i < input.length(); i++) {
    output[i] ^= key[i % key.length()];
  }
  return output;
}

// ---------- ROOT UI ----------
void handleRoot() {
  String page = R"====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#1f1f1f">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="mobile-web-app-capable" content="yes">

<style>
html, body {
  height: 100%;
  margin: 0;
  overflow: hidden;
  font-family: Arial;
  background:#121212;
  color:white;
}

.header {
  background:#1f1f1f;
  color:white;
  padding:10px;
  display:flex;
  justify-content:space-between;
}

#chat {
  padding:10px;
  height: calc(100vh - 120px);
  overflow-y:auto;
}

.bubble {
  padding:10px;
  margin:5px;
  border-radius:10px;
  max-width:70%;
}

.me {
  background:#056162;
  margin-left:auto;
}

.other {
  background:#262d31;
  margin-right:auto;
}

.inputBox {
  display:flex;
  position:fixed;
  bottom:0;
  width:100%;
}

input {
  flex:1;
  padding:10px;
  border:none;
  background:#2a2a2a;
  color:white;
}

button {
  padding:10px;
  background:#056162;
  color:white;
  border:none;
}

.signal {
  display:flex;
  align-items:flex-end;
}

.bar {
  width:5px;
  margin:1px;
  background:gray;
  animation:blink 1s infinite alternate;
}

.bar1{height:5px}
.bar2{height:10px}
.bar3{height:15px}
.bar4{height:20px}
.bar5{height:25px}

.active { background:limegreen;}

@keyframes blink {
  from{opacity:0.5}
  to{opacity:1}
}
</style>
</head>

<body>

<div class="header">
  <div>nR-gram</div>
  <div class="signal">
    <div class="bar bar1" id="b1"></div>
    <div class="bar bar2" id="b2"></div>
    <div class="bar bar3" id="b3"></div>
    <div class="bar bar4" id="b4"></div>
    <div class="bar bar5" id="b5"></div>
  </div>
</div>

<div id="chat"></div>
<div id="typing" style="padding:5px;color:gray;"></div>

<div class="inputBox">
  <input id="msg" placeholder="Type message...">
  <button onclick="sendMsg()">Send</button>
</div>

<script>
function sendMsg() {
  let msg = document.getElementById("msg").value;
  fetch("/send?msg=" + encodeURIComponent(msg));
  document.getElementById("msg").value = "";
}

document.getElementById("msg").addEventListener("input", function() {
  fetch("/typing");
});

function updateChat() {
  fetch("/chat")
  .then(r=>r.text())
  .then(data=>{
    let p = data.split("|");

    let chatDiv = document.getElementById("chat");
    chatDiv.innerHTML = p[0];
    chatDiv.scrollTop = chatDiv.scrollHeight;

    let lvl = parseInt(p[1]);
    for(let i=1;i<=5;i++){
      document.getElementById("b"+i).classList.remove("active");
      if(i<=lvl) document.getElementById("b"+i).classList.add("active");
    }

    document.getElementById("typing").innerHTML = (p[2]=="1")?"Typing...":"";
  });
}

setInterval(updateChat,1000);
</script>

</body>
</html>
)====";

  server.send(200,"text/html",page);
}

// ---------- SEND ----------
void handleSend() {
  String msg = server.arg("msg");

  msgId++;
  String fullMsg = String(msgId) + ":" + msg;

  chatLog += "<div class='bubble me' id='m"+String(msgId)+"'>"+msg+" ✔</div>";

  String enc = encryptDecrypt(fullMsg);

  char payload[32];
  enc.toCharArray(payload, sizeof(payload));

  radio.stopListening();
  bool ok = radio.write(&payload, sizeof(payload));
  radio.startListening();

  totalCount++;
  if(ok) successCount++;

  int q = (successCount*100)/totalCount;
  signalLevel = (q>80)?5:(q>60)?4:(q>40)?3:(q>20)?2:1;

  server.send(200,"text/plain","OK");
}

// ---------- TYPING ----------
void handleTyping() {
  String enc = encryptDecrypt("__typing__");

  char payload[32];
  enc.toCharArray(payload, sizeof(payload));

  radio.stopListening();
  radio.write(&payload, sizeof(payload));
  radio.startListening();

  server.send(200,"text/plain","OK");
}

// ---------- CHAT ----------
void handleChat() {
  server.send(200,"text/plain",
    chatLog+"|"+String(signalLevel)+"|"+String(isTyping));
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid,password);

  radio.begin();
  radio.setPayloadSize(32);   // FIXED SIZE
  radio.openWritingPipe(address);
  radio.openReadingPipe(1,address);
  radio.startListening();

  server.on("/",handleRoot);
  server.on("/send",handleSend);
  server.on("/chat",handleChat);
  server.on("/typing",handleTyping);

  server.begin();
}

// ---------- LOOP ----------
void loop() {
  server.handleClient();

  if(radio.available()){
    char text[32];
    radio.read(&text, sizeof(text));

    String d = encryptDecrypt(String(text));

    if(d=="__typing__"){
      isTyping = true;
      lastTypingTime = millis();
      return;
    }

    if(d.startsWith("ACK:")){
      String id = d.substring(4);
      chatLog.replace("✔</div>", "✔✔</div>");
      return;
    }

    int s = d.indexOf(':');
    if(s>0){
      String id = d.substring(0,s);
      String msg = d.substring(s+1);

      chatLog += "<div class='bubble other'>"+msg+"</div>";

      String ack = encryptDecrypt("ACK:"+id);

      char payload[32];
      ack.toCharArray(payload, sizeof(payload));

      radio.stopListening();
      radio.write(&payload, sizeof(payload));
      radio.startListening();
    }

    isTyping = false;
  }

  // typing timeout
  if(isTyping && millis() - lastTypingTime > 2000){
    isTyping = false;
  }
}