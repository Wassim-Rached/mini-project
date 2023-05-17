// Sketch pour piloter la led integree a la Wemos D1 mini depuis le reseau WiFi

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>  // Include the DHT library
#define DHTPIN 13  // DHT sensor pin connected to GPIO5 (D1)
#define DHTTYPE DHT11  // Change this to DHT22 if you're using a DHT22 sensor

const char* ssid = "WASSIM 3090";          // Identifiant du WiFi que vous utilisez
const char* password = "03sX078(";  // Mot de passe WiFi que vous utilisez
ESP8266WebServer server(80);        // On demande un serveur qui ecoute sur le port 80

int led = 2; // La diode LED intégrée à la carte est branchée sur la broche GPIO2 (D4)
String  etatGPIO2 = "ON";

DHT dht(DHTPIN, DHTTYPE);  // Initialize the DHT sensor

//Ecrire une page Web HTML
String WebPage(){
  String page = R"(
    <!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <!--start font-->
    <link rel="preconnect" href="https://fonts.googleapis.com" />
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
    <link
      href="https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap"
      rel="stylesheet"
    />
    <!--end font-->

    <link
      rel="stylesheet"
      href="https://cdn.jsdelivr.net/npm/@fortawesome/fontawesome-free@6.2.1/css/fontawesome.min.css"
      integrity="sha384-QYIZto+st3yW+o8+5OHfT6S482Zsvz2WfOzpFSXMF9zqeLcFV0/wlZpMtyFcZALm"
      crossorigin="anonymous"
    />
    <title>Page</title>
    <script></script>
    <style>
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
      }
      body {
        font-family: "Poppins", sans-serif;
        position: relative;
        min-height: 100vh;
      }
      #admin_page {
        display: none;
      }
      #loader {
        position: absolute;
        left: 0;
        right: 0;
        bottom: 0;
        top: 0;
        background: red;
      }

      #message {
        text-align: center;
        color: red;
        font-style: italic;
        font-size: small;
        height: 20px;
      }

      form {
        padding-top: 200px;
      }

      .form-box {
        width: 500px;
        background: rgba(0, 0, 0, 0.8);
        margin: auto;
        padding: 50px 0;
        color: #fff;
        box-shadow: 0 0 20px 2px rgba(0, 0, 0, 0.5);
      }
      h1 {
        text-align: center;
      }
      .input-box {
        margin: 15px auto;
        width: 80%;
        border-bottom: 1px solid #fff;
        padding: 8px 0;
      }
      .input-box input {
        width: 90%;
        border: none;
        outline: none;
        background: transparent;
        color: #fff;
      }

      ::placeholder {
        color: #ccc;
      }
      .login-btn {
        margin: 40px auto 20px;
        width: 80%;
        display: block;
        outline: none;
        padding: 10px 0;
        border: 1px solid #fff;
        cursor: pointer;
        background: transparent;
        color: #fff;
        font-size: 16px;
      }
    </style>
  </head>
  <body>
    <script type="module">
      import { initializeApp } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-app.js";
      import {
        getAuth,
        onAuthStateChanged,
        signInWithEmailAndPassword,
        signOut,
      } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-auth.js";

      //Your web app's Firebase configuration
      const firebaseConfig = {
        apiKey: "AIzaSyBOVdBVt41Gqil-iR94E36sys3-zvwn93s",
        authDomain: "mini-projet-6d617.firebaseapp.com",
        projectId: "mini-projet-6d617",
        storageBucket: "mini-projet-6d617.appspot.com",
        messagingSenderId: "292278659148",
        appId: "1:292278659148:web:2ca6feb4f66f59e075ece9",
        measurementId: "G-TVBKEYNMB9",
      };

      //Initialize Firebase
      const app = initializeApp(firebaseConfig);
      const auth = getAuth(app);

      const loginForm = document.f;
      const login_btn = document.getElementById("login-btn");
      const message = document.getElementById("message");

      loginForm.addEventListener("submit", (e) => {
        e.preventDefault();
        login_btn.value = "Login...";
        login_btn.disabled = true;
        //login_btn.value = 'login...';
        //login_btn.disabled = true;
        message.value = "You are logged in";
        const email = loginForm.email.value;
        const password = loginForm.password.value;

        signInWithEmailAndPassword(auth, email, password)
          .then((userCredential) => {
            //User signed in successfully...
            const user = userCredential.user;
            console.log("Logged in user:", user);
            login_btn.value = "Login";
            login_btn.disabled = false;
          })
          .catch((error) => {
            //Handle login errors...
            console.error("Login failed:", error);
            message.innerText = "invalid email or password";
            login_btn.value = "Login";
            login_btn.disabled = false;
          });
      });
      const loader = document.getElementById("loader");
      const admin_page = document.getElementById("admin_page");
      const authentication_page = document.getElementById(
        "authentication_page"
      );
      const username_div = document.getElementById("username");

      onAuthStateChanged(auth, (user) => {
        if (user) {
          //User is logged in, show the protected form
          loader.style.display = "none";
          admin_page.style.display = "block";
          authentication_page.style.display = "none";
          username_div.innerHTML = user.email.replace("@gmail.com", "");
        } else {
          //User is not logged in, hide the protected form
          loader.style.display = "none";
          admin_page.style.display = "none";
          authentication_page.style.display = "block";
        }
      });

      //Handle logout button click
      const logoutButton = document.getElementById("logout-btn");

      logoutButton.addEventListener("click", () => {
        signOut(auth);
      });

      function submitForm() {
        document.getElementById("myForm").submit();
      }
    </script>

    <div id="loader">loading...</div>
    <div id="authentication_page">
      <form name="f">
        <div class="form-box">
          <h1>Login</h1>
          <p id="message"></p>
          <div class="input-box">
            <input type="email" name="email" placeholder="Email" required />
          </div>
          <div class="input-box">
            <input
              type="password"
              name="password"
              placeholder="Password"
              required
            />
          </div>
          <button type="submit" class="login-btn" id="login-btn">Login</button>
        </div>
      </form>
    </div>

    <div id="admin_page">
      <h1>ISET CHARGUIA</h1>
      <div id="user-content">
        <div class="username" id="username"></div>
        <button id="logout-btn">logout</button>
      </div>
      <form method="GET" id="myForm" name="myForm">
        <div id="command-etat">
          <h3>Commande d'une LED sur GPIO</h3>
          <div id="etat">
            GPIO2 :
            <span>)";
             page+=etatGPIO2;
              page+=R"(
            </span>
          </div>
        </div>
        <label>
          <input type='radio' name='GPIO2' value='1' id='ON' )";
           if (etatGPIO2 == "ON"){
             page+="checked";
              } 
              page+= R"(
                />
          <span>ON</span>
        </label>
        <label>
          <input type='radio' name='GPIO2' value='0')"; 
          if (etatGPIO2 == "OFF"){
          page+="checked"; 
          } 
          page+=R"(
             />
          <span> OFF</span>
        </label>
      </form>
      <br /><br />
      <p id="iset_ch"><a href="#">ISET Charguia &copy;</a></p>
      <p id="admin_name"></p>
    </div>
    <div class='dht-container'>
	<div class="temperature">
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 320 512"><path d="M16
   0 64c-26.5 0-48 21.5-48 48V276.5c0 17.3-7.1 31.9-15.3 42.5C86.2 332.6 80 349.5 80 368c0 44.2 35.8 80 80 80s80-35.8 80-80c0-18.5-6.2-35.4-16.7-48.9c-8.2-10.6-15.3-25.2-15.3-42
   .5V112c0-26.5-21.5-48-48-48zM48 112C48 50.2 98.1 0 160 0s112 50.1 112 112V276.5c0 .1 .1 .3 .2 .6c.2 .6 .8 1.6 1.7 2.8c18.9 24.4 30.1 55 30.1 88.1c0 79.5-64.5 144-144 144S16 447.5 16 368c0-33.2 11.2-63.8 30.1-88.1c.9-1.2 1.5-2.2 1.7-2.8c.1-.3 .2-.5 .2-.6V112zM208 368c0 26.5-21.5 48-48 48s-48-21.5-48-48c0-20.9 13.4-38.7 32-45.3V208c0-8.8 7.2-16 16-16s16 7.2 16 16V322.7c18.6 6.6 32 24.4 32 45.3z"/></svg>
		<span>temperature</span>
    <span>43</span>
    <span>°C</span>
	</div>
	<div class="lumidity">
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 384 512"><path d="M192 512C86 512 0 426 0 320C0 228.8 130.2 57.7 166.6 11.7C172.6 4.2 181.5 0 191.1 0h1.8c9.6 0 18.5 4.2 24.5 11.7C253.8 57.7 384 228.8 384 320c0 106-86 192-192 192zM96 336c0-8.8-7.2-16-16-16s-16 7.2-16 16c0 61.9 50.1 112 112 112c8.8 0 16-7.2 16-16s-7.2-16-16-16c-44.2 0-80-35.8-80-80z"/></svg>
    <span>lumidity</span>
    <span>43</span>
    <span>%</span>
	</div>
</div>
  </body>
</html>
  )";


return page;
}



void Soumettre() {
  // Actualise le GPIO 
if (server.method() == HTTP_POST) {
    // Process the POST request here
    String GPIOValue = server.arg("GPIO2");

    Serial.print("Set GPIO to ");
    Serial.println(GPIOValue);

    if (GPIOValue == "1") {
      digitalWrite(2, LOW);
      etatGPIO2 = "ON";
    } else if (GPIOValue == "0") {
      digitalWrite(2, HIGH);
      etatGPIO2 = "OFF";
    } else {
      server.send(400, "application/json", "{\"status\": \"error\", \"message\": \"Invalid GPIO value\"}");
      return;
    }

    server.send(200, "application/json", "{\"state\": \"" + etatGPIO2 + "\"}");
  }

  // Read DHT sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
}

void Racine(){ 
  if (server.hasArg("GPIO2") && server.method() == HTTP_POST ) {
    Soumettre();
  } else {
    server.send ( 200, "text/html", WebPage() );
  }  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  
  // on attend d'etre connecté au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  // on affiche l'adresse IP qui nous a ete attribuée
  Serial.println("");
  Serial.print("Addresse IP : ");
  Serial.println(WiFi.localIP());
  // on definit les points d'entree (les URL a saisir dans le navigateur web) et on affiche un simple texte
  server.on ( "/", Racine );

  // on demarre le serveur web
  server.begin();
  pinMode(led, OUTPUT);     // Initialise la broche GPIO2 comme une sortie

  dht.begin();  // Initialize the DHT sensor
}

// Cette boucle s'exécute à l'infini
void loop() {
    // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();

  
  // Read DHT sensor data periodically
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the sensor reading is valid
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}
